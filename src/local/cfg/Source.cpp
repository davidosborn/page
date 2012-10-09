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

#ifndef USE_WIN32
#	include "../sys.hpp" // GetUser
#endif
#include "../util/path.hpp" // GetExt
#include "../util/pp.hpp" // STRINGIZE
#include "Source.hpp"
#include "source/ConfSource.hpp"
#include "source/IniSource.hpp"
#ifdef USE_WIN32
#	include "source/win32/RegSource.hpp"
#endif

// HACK: for demo
//#include "../err/exception/throw.hpp" // THROW

namespace page
{
	namespace cfg
	{
		Source::~Source() {}

		Reader::Reader(const Source &source) : source(source) {}
		Reader::~Reader() {}

		Writer::Writer(const Source &source) : source(source) {}
		Writer::~Writer() {}

		Source *MakeSiteSource()
		{
#ifdef USE_WIN32
			return new win32::RegSource(HKEY_LOCAL_MACHINE, "Software\\" STRINGIZE(NAME));
#else
			return new ConfSource("/etc/" STRINGIZE(PACKAGE) ".conf");
#endif
			// HACK: for demo
//			return new ConfSource("conf");
		}
		Source *MakeUserSource()
		{
#ifdef USE_WIN32
			return new win32::RegSource(HKEY_CURRENT_USER, "Software\\" STRINGIZE(NAME));
#else
			return new ConfSource(std::string("/home/") + sys::GetUser() + "/." STRINGIZE(PACKAGE) ".conf");
#endif
			// HACK: for demo
//			THROW err::Exception<err::DeliberateTag>("no user source");
		}
		Source *MakeFileSource(const std::string &path)
		{
			std::string ext(util::GetExt(path));
			if (ext == "ini") return new IniSource(path);
			return new ConfSource(path);
		}
	}
}
