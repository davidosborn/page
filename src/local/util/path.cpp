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

// C++
#include <algorithm> // find, remove, remove_if, transform
#include <cctype> // isdigit
#include <ctime> // {,local}time
#include <functional> // bind, logical_and, logical_not
#include <locale> // use_facet, time_put
#include <sstream> // ostringstream
#include <utility> // pair

// Boost
#include <boost/filesystem/path.hpp>

// local
#include "../err/Exception.hpp"
#include "functional/locale.hpp" // tolower_function

#include "path.hpp"

namespace page
{
	namespace util
	{
		/*------------------+
		| path manipulation |
		+------------------*/

		boost::filesystem::path ExpandPath(const boost::filesystem::path &path)
		{
			/*std::string expPath(path);
			// remove null characters
			expPath.erase(std::remove(expPath.begin(), expPath.end(), '\0'), expPath.end());
			// hide incremental symbols
			bool hasInc = false;
			for (std::string::iterator iter(expPath.begin());
				(iter = std::find(iter, expPath.end(), '%')) != expPath.end(); ++iter)
				for (std::string::iterator delim(iter++); iter != expPath.end(); ++iter)
					if (!std::isdigit(*iter))
					{
						if (*iter == 'i')
						{
							*delim = '\0';
							hasInc = true;
						}
						break;
					}
			// expand date/time symbols
			std::time_t time = std::time(0);
			std::ostringstream ss;
			std::use_facet<std::time_put<char>>(ss.getloc()).put(
				ss, ss, ss.fill(), std::localtime(&time),
				&expPath[0], &expPath[0] + expPath.size());
			expPath = ss.str();
			// remove invalid characters
			expPath.erase(std::remove_if(expPath.begin(), expPath.end(),
				std::bind(std::logical_and<bool>(), std::placeholders::_1,
				std::bind(std::logical_and<bool>(), sys::IsInvalid,
				std::bind(std::logical_not<bool>(), sys::IsWildcard)))), expPath.end());
			// expand incremental symbols
			if (hasInc)
			{
				for (unsigned n = 0;; n++)
				{
					std::string incPath(expPath);
					std::string ns(lexical_cast<std::string>(n));
					for (std::streamsize i = incPath.size() - 1;
						(i = incPath.rfind('\0', i)) != std::string::npos; i--)
					{
						std::streamsize j = i + 1, k = incPath.find('i', j);
						if (ns.empty())
						{
							incPath.replace(i, k - i + 1, "0");
							continue;
						}
						if (j == k)
						{
							incPath.replace(i, k - i + 1, ns);
							ns.clear();
						}
						else
						{
							unsigned width = lexical_cast<unsigned>(incPath.substr(j, k - j));
							if (width < ns.size())
							{
								std::streamsize nsi = ns.size() - width;
								incPath.replace(i, k - i + 1, ns.substr(nsi));
								ns.erase(nsi);
							}
							else
							{
								ns.insert(0, width - ns.size(), '0');
								incPath.replace(i, k - i + 1, ns);
								ns.clear();
							}
						}
					}
					if (!ns.empty())
						THROW((err::Exception<err::UtilModuleTag, err::RangeTag>("incremental path out of range")))
					if (!sys::PathExists(incPath))
					{
						expPath = incPath;
						break;
					}
				}
			}
			return expPath;*/

			return path;
		}

		/*-----------------------+
		| extension manipulation |
		+-----------------------*/

		boost::filesystem::path GetExtension(
			const boost::filesystem::path &path,
			unsigned index)
		{
			auto s(path.native());
			decltype(s)::size_type extStart, extEnd(s.npos);
			for (;;)
			{
				extStart = s.rfind('.', extEnd);
				if (!index || extStart == s.npos) break;
				extEnd = extStart;
				--index;
			}
			if (extStart)
			{
				auto ext(s.substr(extStart + 1, extEnd));
				std::transform(ext.begin(), ext.end(), ext.begin(), tolower_function<char>());
				return boost::filesystem::path(ext);
			}
			return boost::filesystem::path();
		}

		std::pair<boost::filesystem::path, boost::filesystem::path>
			PartitionExtension(const boost::filesystem::path &path)
		{
			auto s(path.native());
			auto i(s.rfind('.'));
			return i != s.npos ?
				std::make_pair(
					boost::filesystem::path(s.substr(0, i)),
					boost::filesystem::path(s.substr(i + 1))) :
				std::make_pair(
					path,
					boost::filesystem::path());
		}

		boost::filesystem::path AddExtension(
			const boost::filesystem::path &path,
			const boost::filesystem::path &extension)
		{
			boost::filesystem::path newPath(path);
			if (newPath.empty() || newPath.native().back() != '.')
				newPath += '.';
			newPath += extension;
			return newPath;
		}

		boost::filesystem::path WithExtension(
			const boost::filesystem::path &path,
			const boost::filesystem::path &extension)
		{
			if (GetExtension(path) != extension)
				return AddExtension(path, extension);
			return path;
		}
	}
}
