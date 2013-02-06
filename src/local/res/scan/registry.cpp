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
#include "../../util/path.hpp" // GetExt
#include "../../util/serialize/deserialize_string.hpp" // Deserialize
#include "../Node.hpp"
#include "../pipe/Pipe.hpp"
#include "callback.hpp" // ScanCallback
#include "function.hpp" // ScanFunction

namespace page
{
	namespace res
	{
		/**
		 * The lock size to use for preloading (an optimization).
		 *
		 * @note Some scanners require larger lock sizes.
		 * @note Tar puts its 8 byte format identifier at offset 257, so it
		 *       needs a lock size of 265.
		 * @note The FreeType BDF module reads in 1024 byte increments, so it
		 *       needs a lock size of 1024.
		 */
		const unsigned lockSize = 1024;

		namespace
		{
			struct Registry
			{
				typedef std::vector<ScanFunction> Scanners;
				typedef std::unordered_multimap<std::string, ScanFunction> Associations;

				Scanners scanners;
				Associations exts, mimes, tags;
			};
			inline Registry &GetRegistry()
			{
				static Registry reg;
				return reg;
			}
		}

		// access
		bool CallRegisteredScanner(const Node &node, const ScanCallback &cb)
		{
			PipeLocker locker(*node.pipe, lockSize);
			const Registry &reg(GetRegistry());
			// check tag
			if (!node.tag.empty())
			{
				auto range(util::make_range(reg.tags.equal_range(node.tag)));
				for (const auto &assoc : range)
				{
					ScanFunction scan(assoc.second);
					if (scan(node.pipe, cb)) return true;
				}
				if (!range.empty())
					THROW((err::Exception<err::ResModuleTag, err::NotFoundTag>("mismatched tag") <<
						boost::errinfo_file_name(node.path) <<
						err::errinfo_subject(node.tag)))
			}
			// check mime type
			if (!node.mime.empty())
			{
				auto range(util::make_range(reg.mimes.equal_range(node.mime)));
				for (const auto &assoc : range)
				{
					ScanFunction scan(assoc.second);
					if (scan(node.pipe, cb)) return true;
				}
				if (!range.empty())
					THROW((err::Exception<err::ResModuleTag, err::NotFoundTag>("mismatched mime type") <<
						boost::errinfo_file_name(node.path) <<
						err::errinfo_subject(node.mime)))
			}
			// check extension
			std::string ext(util::GetExt(node.path));
			if (!ext.empty())
			{
				auto range(util::make_range(reg.exts.equal_range(ext)));
				for (const auto &assoc : range)
				{
					ScanFunction scan(assoc.second);
					if (scan(node.pipe, cb)) return true;
				}
				if (!range.empty())
					THROW((err::Exception<err::ResModuleTag, err::NotFoundTag>("mismatched extension") <<
						boost::errinfo_file_name(node.path) <<
						err::errinfo_subject(ext)))
			}
			// resort to format inspection
			if (node.inspect)
				for (const auto &scanner : reg.scanners)
					if (scanner(node.pipe, cb)) return true;
			return false;
		}

		// registration
		void RegisterScanner(const ScanFunction &scan, const std::string &ext, const std::string &mime, const std::string &tag, bool inspect)
		{
			Registry &reg(GetRegistry());
			if (inspect) reg.scanners.push_back(scan);
			// register extensions
			std::vector<std::string> exts;
			util::Deserialize(ext, std::back_inserter(exts), util::SequenceDeserializationFlags::none, ',');
			for (const auto &ext : exts)
				reg.exts.insert(std::make_pair(ext, scan));
			// register mime types
			std::vector<std::string> mimes;
			util::Deserialize(mime, std::back_inserter(mimes), util::SequenceDeserializationFlags::none, ',');
			for (const auto &mime : mimes)
				reg.mimes.insert(std::make_pair(mime, scan));
			// register tags
			std::vector<std::string> tags;
			util::Deserialize(tag, std::back_inserter(tags), util::SequenceDeserializationFlags::none, ',');
			for (const auto &tag : tags)
				reg.tags.insert(std::make_pair(tag, scan));
		}
	}
}
