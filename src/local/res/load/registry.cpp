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
 *
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

#include <iterator> // back_inserter
#include <unordered_map> // unordered_multimap
#include <vector>

#include "../../err/Exception.hpp"
#include "../../util/iterator/range.hpp"
#include "../../util/path/extension.hpp" // GetExtension
#include "../../util/serialize/deserialize_string.hpp" // Deserialize
#include "../Node.hpp"
#include "function.hpp" // GetLoaderFunction, LoadFunction

namespace page
{
	namespace res
	{
		namespace
		{
			struct Registry
			{
				struct Type
				{
					typedef std::vector<GetLoaderFunction> Loaders;
					typedef std::unordered_multimap<std::string, GetLoaderFunction> Associations;

					Loaders loaders;
					Associations exts, mimes, tags;
				};
				typedef std::unordered_map<std::string, Type> Types;
				Types types;
			};
			inline Registry &GetRegistry()
			{
				static Registry reg;
				return reg;
			}
		}

		// access
		LoadFunction GetRegisteredLoader(const std::type_info &id, const Node &node)
		{
			const Registry &reg(GetRegistry());
			Registry::Types::const_iterator iter(reg.types.find(id.name()));
			if (iter != reg.types.end())
			{
				const Registry::Type &type(iter->second);
				// check tag
				if (!node.tag.empty())
				{
					auto range(util::make_range(type.tags.equal_range(node.tag)));
					for (const auto &assoc : range)
					{
						const GetLoaderFunction &loader(assoc.second);
						LoadFunction load(loader(*node.pipe));
						if (load) return load;
					}
					if (!range.empty())
						THROW((err::Exception<err::ResModuleTag, err::NotFoundTag>("mismatched tag") <<
							boost::errinfo_file_name(node.path) <<
							err::errinfo_subject(node.tag)))
				}
				// check mime type
				if (!node.mime.empty())
				{
					auto range(util::make_range(type.mimes.equal_range(node.mime)));
					for (const auto &assoc : range)
					{
						const GetLoaderFunction &loader(assoc.second);
						LoadFunction load(loader(*node.pipe));
						if (load) return load;
					}
					if (!range.empty())
						THROW((err::Exception<err::ResModuleTag, err::NotFoundTag>("mismatched mime type") <<
							boost::errinfo_file_name(node.path) <<
							err::errinfo_subject(node.mime)))
				}
				// check extension
				std::string ext(util::GetExtension(node.path));
				if (!ext.empty())
				{
					auto range(util::make_range(type.exts.equal_range(ext)));
					for (const auto &assoc : range)
					{
						const GetLoaderFunction &loader(assoc.second);
						LoadFunction load(loader(*node.pipe));
						if (load) return load;
					}
					if (!range.empty())
						THROW((err::Exception<err::ResModuleTag, err::NotFoundTag>("mismatched extension") <<
							boost::errinfo_file_name(node.path) <<
							err::errinfo_subject(ext)))
				}
				// resort to format inspection
				if (node.inspect)
					for (const auto &loader : type.loaders)
					{
						LoadFunction load(loader(*node.pipe));
						if (load) return load;
					}
			}
			return 0;
		}

		// registration
		void RegisterLoader(const std::type_info &id, const GetLoaderFunction &load, const std::string &ext, const std::string &mime, const std::string &tag, bool inspect)
		{
			Registry &reg(GetRegistry());
			Registry::Type &type(reg.types.insert(std::make_pair(id.name(), Registry::Type())).first->second);
			if (inspect) type.loaders.push_back(load);
			// register extensions
			std::vector<std::string> exts;
			util::Deserialize(ext, std::back_inserter(exts), util::SequenceDeserializationFlags::none, ',');
			for (const auto &ext : exts)
				type.exts.insert(std::make_pair(ext, load));
			// register mime types
			std::vector<std::string> mimes;
			util::Deserialize(mime, std::back_inserter(mimes), util::SequenceDeserializationFlags::none, ',');
			for (const auto &mime : mimes)
				type.mimes.insert(std::make_pair(mime, load));
			// register tags
			std::vector<std::string> tags;
			util::Deserialize(tag, std::back_inserter(tags), util::SequenceDeserializationFlags::none, ',');
			for (const auto &tag : tags)
				type.tags.insert(std::make_pair(tag, load));
		}
	}
}
