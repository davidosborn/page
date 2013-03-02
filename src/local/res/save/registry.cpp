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

#include <iostream> // clog
#include <string>
#include <typeinfo> // type_info
#include <unordered_map>
#include <utility> // pair
#include <vector>

#include "../../err/Exception.hpp"
#include "../../log/manip.hpp" // Warning
#include "../../util/io/deserialize.hpp" // Deserialize
#include "../../util/path/extension.hpp" // {Get,With}Extension
#include "../type/Registry.hpp"
#include "function.hpp" // SaveFunction

namespace page
{
	namespace res
	{
		namespace
		{
			struct Registry
			{
				struct Saver
				{
					SaveFunction save;
					typedef std::vector<std::string> Exts;
					Exts exts;
					unsigned rank;
				};
				typedef std::unordered_map<std::string, Saver> Associations;
				struct Type
				{
					Saver defaultSaver;
					Associations exts, fmts;
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
		std::pair<SaveFunction, std::string> GetRegisteredSaver(const std::type_info &id, const std::string &path, const std::string &fmt)
		{
			const Registry &reg(GetRegistry());
			Registry::Types::const_iterator iter(reg.types.find(id.name()));
			if (iter != reg.types.end())
			{
				const Registry::Type &type(iter->second);
				// check format
				if (!fmt.empty())
				{
					Registry::Associations::const_iterator iter(type.fmts.find(fmt));
					if (iter != type.fmts.end())
					{
						const Registry::Saver &saver(iter->second);
						return std::make_pair(saver.save,
							util::WithExtension(path,
								saver.exts.begin(),
								saver.exts.end()));
					}
					std::clog << log::Warning << "unknown " << GLOBAL(type::Registry).Query(id).name << " format: " << fmt << std::endl;
				}
				// check extension
				std::string ext(util::GetExtension(path));
				if (!ext.empty())
				{
					Registry::Associations::const_iterator iter(type.exts.find(ext));
					if (iter != type.exts.end())
					{
						const Registry::Saver &saver(iter->second);
						return std::make_pair(saver.save, path);
					}
				}
				// use default saver
				// NOTE: given a matching type, there must be a default saver
				return std::make_pair(type.defaultSaver.save,
					util::WithExtension(path,
						type.defaultSaver.exts.begin(),
						type.defaultSaver.exts.end()));
			}
			THROW((err::Exception<err::ResModuleTag, err::NotAvailableTag>("no " + GLOBAL(type::Registry).Query(id).name + " savers available")))
		}

		// registration
		void RegisterSaver(const std::type_info &id, const SaveFunction &save, const std::string &ext, const std::string &fmt, unsigned rank)
		{
			Registry &reg(GetRegistry());
			Registry::Type &type(reg.types.insert(std::make_pair(id.name(), Registry::Type())).first->second);
			// parse extensions for saver
			typedef std::vector<std::string> Attributes;
			Attributes exts;
			util::Deserialize(ext, std::back_inserter(exts), util::SequenceDeserializationFlags::none, ',');
			// create saver
			Registry::Saver saver = {save, exts, rank};
			// register default saver
			if (saver.rank > type.defaultSaver.rank)
				type.defaultSaver = saver;
			// register extensions
			for (Attributes::const_iterator ext(exts.begin()); ext != exts.end(); ++ext)
			{
				Registry::Saver &extSaver(type.exts.insert(std::make_pair(*ext, saver)).first->second);
				if (saver.rank > extSaver.rank) extSaver = saver;
			}
			// register formats
			Attributes fmts;
			util::Deserialize(fmt, std::back_inserter(fmts), util::SequenceDeserializationFlags::none, ',');
			for (Attributes::const_iterator fmt(fmts.begin()); fmt != fmts.end(); ++fmt)
			{
				Registry::Saver &fmtSaver(type.fmts.insert(std::make_pair(*fmt, saver)).first->second);
				if (saver.rank > fmtSaver.rank) fmtSaver = saver;
			}
		}
	}
}
