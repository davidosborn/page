#include <functional> // bind
#include <iostream> // cout
#include <regex>

#include <boost/optional.hpp>

#include "../../cache.hpp" // PurgeResource
#include "../../cfg/vars.hpp"
#include "../../err/Exception.hpp"
#include "../../err/report.hpp" // ReportWarning, std::exception
#include "../../log/Indenter.hpp"
#include "../load/LoaderRegistry.hpp"
#include "../node/path.hpp" // CatPath
#include "../pipe/Pipe.hpp" // Pipe::Open
#include "../scan/ScannerRegistry.hpp"
#include "../type/TypeRegistry.hpp"
#include "Source.hpp"

namespace page { namespace res
{
	/*----------------------------------------+
	| constructors & special member functions |
	+----------------------------------------*/

	Source::Source(const std::string &path) :
		path(path) {}

	/*----------------------------------+
	| to be overridden by derived class |
	+----------------------------------*/

	bool Source::CheckPath(const std::string &path)
	{
		assert(!"function must be overridden by derived class");
	}

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
			std::shared_ptr<const void> resource(LoadFromDisk(id, path), GLOBAL(TypeRegistry).Query(id).deleter);
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
			std::shared_ptr<const void> resource(LoadFromDisk(id, path), GLOBAL(TypeRegistry).Query(id).deleter);
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
				resource.reset(LoadFromDisk(id, path), GLOBAL(TypeRegistry).Query(id).deleter);
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
				LoadFunction loader(GLOBAL(load::Registry).GetLoader(id, path.node));
				if (!loader)
					THROW((err::Exception<err::ResModuleTag, err::NotFoundTag>("resource type mismatch") <<
						boost::errinfo_file_name(path.node.path) <<
						err::errinfo_subject(id.name)))
				Path::Type pathType = {loader};
				iter = path.types.insert(std::make_pair(id.name(), pathType)).first;
			}
			const Path::Type &type(iter->second);
			void *data = type.loader(path.node.pipe);
			if (!data)
				THROW((err::Exception<err::ResModuleTag, err::NotFoundTag>("resource type mismatch") <<
					boost::errinfo_file_name(path.node.path) <<
					err::errinfo_subject(id.name)))
			auto postLoader(GLOBAL(TypeRegistry).Query(id).postLoader);
			if (postLoader) postLoader(data);
			return data;
		}
		return 0;
	}
	void Source::ScanToBuildIndex(const Node &node, Group &group, const std::string &rootPath)
	{
		std::string path(CatPath(rootPath, node.path));
		// filter path
		for (const auto &filter : *CVAR(resourceExcludes))
		{
			/**
			 * @todo GCC does not implement @c regex_search yet, so we
			 *       emulate it via @c regex_match and leading/trailing
			 *       wildcards.
			 */
			if (std::regex_match(path, std::regex(".*" + filter + ".*")))
				return;
		}
		// print path
		boost::optional<log::Indenter> indenter;
		if (*CVAR(logVerbose))
		{
			std::cout << (!node.path.empty() ? path : std::string(".")) << std::endl;
			indenter = boost::in_place();
		}
		// add node to index
		std::pair<Paths::iterator, bool> result(paths.insert(std::make_pair(path, Path())));
		if (result.second) group.paths.push_back(result.first);
		result.first->second.node = node;
		// recurse into node
		try
		{
			GLOBAL(scan::Registry).Scan(node, std::bind(&Source::ScanToBuildIndex, this, std::placeholders::_1, group, path));
		}
		catch (const std::exception &e)
		{
			err::ReportWarning(e);
		}
	}
}}
