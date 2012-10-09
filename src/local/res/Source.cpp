/**
 * @section license
 *
 * Copyright (c) 2006-2012 David Osborn
 *
 * Permission is granted to use and redistribute this software in source and
 * binary form, with or without modification, subject to the following
 * conditions:
 *
 * 1. Redistributions in source form must retain the above copyright notice,
 *    this list of conditions, and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions, and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution, and in the same
 *    place and form as other copyright, license, and disclaimer information.
 *
 * As a special exception, distributions of derivative works in binary form may
 * include an acknowledgement in place of the above copyright notice, this list
 * of conditions, and the following disclaimer in the documentation and/or other
 * materials provided with the distribution, and in the same place and form as
 * other acknowledgements, similar in substance to the following:
 *
 *    Portions of this software are based on the work of David Osborn.
 *
 * This software is provided "as is", without any express or implied warranty.
 * In no event will the authors be liable for any damages arising out of the use
 * of this software.
 */

#include <functional> // bind
#include <iostream> // cout
#include <regex>
#include "../cache.hpp" // PurgeResource
#include "../cfg.hpp" // logVerbose
#include "../err/exception/catch.hpp" // CATCH_ALL_AND_PRINT_WARNING
#include "../err/exception/throw.hpp" // THROW
#include "../log/Indenter.hpp"
#include "load/registry.hpp" // GetRegisteredLoader, LoadFunction
#include "path.hpp" // CatPath
#include "Pipe.hpp" // Pipe::Open
#include "scan/registry.hpp" // CallRegisteredScanner
#include "Source.hpp"
#include "type/registry.hpp" // CallWithPointer, GetRegisteredType{Deleter,PostLoader}, PostLoadFunction

namespace page
{
	namespace res
	{
		// destructor
		Source::~Source() {}

		// modifiers
		void Source::Refresh() {}
		void Source::Index(const Node &node)
		{
			ScanToBuildIndex(node, groups.insert(std::make_pair(node.path, Group())).first->second);
		}
		void Source::Clear(const std::string &group)
		{
			// HACK: GCC produces scope warnings if both iterators have the same
			// name, even when the -ffor-scope flag is given (bug #33260)
			Groups::iterator groupIter(groups.find(group));
			if (groupIter != groups.end())
			{
				Group &group(groupIter->second);
				for (Group::PathPointers::iterator iter(group.paths.begin()); iter != group.paths.end(); ++iter)
				{
					// FIXME: we should only clear the cache if we are the
					// top-level source for the given path
					// FIXME: we should not be purging the cache here anyway!
					cache::PurgeResource((*iter)->first);
					paths.erase(*iter);
				}
				groups.erase(groupIter);
			}
		}

		// resource access
		Stream *Source::Open(const std::string &path) const
		{
			Paths::const_iterator iter(paths.find(path));
			return iter != paths.end() ? iter->second.node.pipe->Open() : 0;
		}
		std::shared_ptr<const void> Source::Load(const std::type_info &id, const std::string &path) const
		{
			// check if path is known
			Paths::iterator pathIter(paths.find(path));
			if (pathIter == paths.end())
			{
				std::shared_ptr<const void> resource(LoadFromDisk(id, path), GetRegisteredTypeDeleter(id));
				if (resource)
				{
					Path::Types &types(paths.insert(std::make_pair(path, Path())).first->second.types);
					Path::Type::Data &data(types.insert(std::make_pair(id.name(), Path::Type())).first->second.data);
					data = resource;
				}
				return resource;
			}
			// check if type is known
			Path::Types &types(pathIter->second.types);
			Path::Types::iterator typeIter(types.find(id.name()));
			if (typeIter == types.end())
			{
				std::shared_ptr<const void> resource(LoadFromDisk(id, path), GetRegisteredTypeDeleter(id));
				if (resource)
				{
					Path::Type::Data &data(types.insert(std::make_pair(id.name(), Path::Type())).first->second.data);
					data = resource;
				}
				return resource;
			}
			// check if resource is loaded
			Path::Type::Data &data(typeIter->second.data);
			std::shared_ptr<const void> resource(data.lock());
			if (!resource)
			{
				try
				{
					resource.reset(LoadFromDisk(id, path), GetRegisteredTypeDeleter(id));
				}
				catch (...)
				{
					types.erase(typeIter);
					if (types.empty()) paths.erase(pathIter);
					throw;
				}
				if (!resource)
				{
					types.erase(typeIter);
					if (types.empty()) paths.erase(pathIter);
				}
				data = resource;
			}
			return resource;
		}

		// parsing
		const void *Source::LoadFromDisk(const std::type_info &id, const std::string &path) const
		{
			Paths::iterator iter(paths.find(path));
			if (iter != paths.end())
			{
				Path &path(iter->second);
				Path::Types::const_iterator iter(path.types.find(id.name()));
				if (iter == path.types.end())
				{
					LoadFunction loader(GetRegisteredLoader(id, path.node));
					if (!loader) THROW err::More(err::Exception<err::NotFoundTag, err::ResourceTag>("resource type mismatch"), path.node.path);
					Path::Type pathType = {loader};
					iter = path.types.insert(std::make_pair(id.name(), pathType)).first;
				}
				const Path::Type &type(iter->second);
				void *data = type.loader(path.node.pipe);
				if (!data) THROW err::More(err::Exception<err::NotFoundTag, err::ResourceTag>("resource type mismatch"), path.node.path);
				PostLoadFunction postLoader(GetRegisteredTypePostLoader(id));
				if (postLoader) CallWithPointer(postLoader, data);
				return data;
			}
			return 0;
		}
		void Source::ScanToBuildIndex(const Node &node, Group &group, const std::string &rootPath)
		{
			std::string path(CatPath(rootPath, node.path));
			// filter path
			for (cfg::ResFilters::const_iterator filter(cfg::resFilters->begin()); filter != cfg::resFilters->end(); ++filter)
			{
				/**
				 * @todo GCC does not implement @c regex_search yet, so we
				 *       emulate it via @c regex_match and leading/trailing
				 *       wildcards.
				 */
				if (std::regex_match(path, std::regex(".*" + *filter + ".*")))
					return;
			}
			// print path
			log::Indenter indenter(false);
			if (*cfg::logVerbose)
			{
				std::cout << (!node.path.empty() ? path : std::string(".")) << std::endl;
				indenter.Reset(true);
			}
			// add node to index
			std::pair<Paths::iterator, bool> result(paths.insert(std::make_pair(path, Path())));
			if (result.second) group.paths.push_back(result.first);
			result.first->second.node = node;
			// recurse into node
			try
			{
				CallRegisteredScanner(node, std::bind(&Source::ScanToBuildIndex, this, std::placeholders::_1, group, path));
			}
			CATCH_ALL_AND_PRINT_WARNING
		}
	}
}
