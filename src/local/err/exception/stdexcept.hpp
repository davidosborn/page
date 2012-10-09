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

#ifndef    page_local_err_exception_stdexcept_hpp
#   define page_local_err_exception_stdexcept_hpp

#	include "Exception.hpp"

#	define DEFINE_EXCEPTION(NAME, WHAT, BASE, ...) \
		template <typename... Tags_> \
			struct NAME##Exception : \
				BASE##Exception<>, \
				BASE##Exception<__VA_ARGS__, Tags_...> \
		{ \
			explicit NAME##Exception(const std::string &what = WHAT) : \
				BASE##Exception<>(what) {} \
			\
			typedef typename BASE##Exception<__VA_ARGS__, Tags_...>::Tags Tags; \
			\
			using BASE##Exception<>::What; \
			using BASE##Exception<>::what; \
		};

namespace page
{
	namespace err
	{
		DEFINE_EXCEPTION(Alloc,        "allocation error",   ,           AllocTag)
		DEFINE_EXCEPTION(Format,       "format error",       ,           FormatTag)

		DEFINE_EXCEPTION(File,         "file error",         ,           FileTag)
		DEFINE_EXCEPTION(FileAccess,   "file access denied", File,       AccessDeniedTag)
		DEFINE_EXCEPTION(FileNotFound, "file not found",     File,       NotFoundTag)
		DEFINE_EXCEPTION(FileRead,     "file read error",    File,       StreamTag, ReadTag)
		DEFINE_EXCEPTION(FileWrite,    "file write error",   File,       StreamTag, WriteTag)
		DEFINE_EXCEPTION(EndOfFile,    "end of file",        FileRead,   EndOfStreamTag)
		DEFINE_EXCEPTION(NotFile,      "not a file",         File,       TypeTag)
		DEFINE_EXCEPTION(NotDirectory, "not a directory",    File,       TypeTag)

		DEFINE_EXCEPTION(Stream,       "stream error",       ,           StreamTag)
		DEFINE_EXCEPTION(StreamRead,   "stream read error",  Stream,     ReadTag)
		DEFINE_EXCEPTION(StreamWrite,  "stream write error", Stream,     WriteTag)
		DEFINE_EXCEPTION(EndOfStream,  "end of stream",      StreamRead, EndOfStreamTag)
	}
}

#endif
