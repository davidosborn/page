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

#include "tags.hpp"

namespace page
{
	namespace err
	{
		/*-----+
		| tags |
		+-----*/

		template <> const char *Tag<TagId::accessDenied> ::name = "access denied";
		template <> const char *Tag<TagId::alloc>        ::name = "allocation";
		template <> const char *Tag<TagId::catastrophic> ::name = "catastrophic";
		template <> const char *Tag<TagId::conversion>   ::name = "conversion";
		template <> const char *Tag<TagId::deliberate>   ::name = "deliberate";
		template <> const char *Tag<TagId::end>          ::name = "end";
		template <> const char *Tag<TagId::file>         ::name = "file";
		template <> const char *Tag<TagId::fileOpen>     ::name = "file open";
		template <> const char *Tag<TagId::format>       ::name = "format";
		template <> const char *Tag<TagId::input>        ::name = "input";
		template <> const char *Tag<TagId::io>           ::name = "io";
		template <> const char *Tag<TagId::key>          ::name = "key";
		template <> const char *Tag<TagId::notAvailable> ::name = "not available";
		template <> const char *Tag<TagId::notDirectory> ::name = "not directory";
		template <> const char *Tag<TagId::notFile>      ::name = "not file";
		template <> const char *Tag<TagId::notFound>     ::name = "not found";
		template <> const char *Tag<TagId::output>       ::name = "output";
		template <> const char *Tag<TagId::parameter>    ::name = "parameter";
		template <> const char *Tag<TagId::parser>       ::name = "parser";
		template <> const char *Tag<TagId::path>         ::name = "path";
		template <> const char *Tag<TagId::platform>     ::name = "platform";
		template <> const char *Tag<TagId::range>        ::name = "range";
		template <> const char *Tag<TagId::readOnly>     ::name = "read only";
		template <> const char *Tag<TagId::serialization>::name = "serialization";
		template <> const char *Tag<TagId::stream>       ::name = "stream";
		template <> const char *Tag<TagId::streamFlush>  ::name = "stream flush";
		template <> const char *Tag<TagId::streamSeek>   ::name = "stream seek";
		template <> const char *Tag<TagId::streamTell>   ::name = "stream tell";
		template <> const char *Tag<TagId::token>        ::name = "token";
		template <> const char *Tag<TagId::type>         ::name = "type";

		/*---------------------+
		| module-specific tags |
		+---------------------*/

		template <> const char *Tag<TagId::audModule>   ::name = "audio module";
		template <> const char *Tag<TagId::cacheModule> ::name = "cache module";
		template <> const char *Tag<TagId::cfgModule>   ::name = "configuration module";
		template <> const char *Tag<TagId::clipModule>  ::name = "clip module";
		template <> const char *Tag<TagId::envModule>   ::name = "environment module";
		template <> const char *Tag<TagId::errModule>   ::name = "error module";
		template <> const char *Tag<TagId::gameModule>  ::name = "game module";
		template <> const char *Tag<TagId::inpModule>   ::name = "input module";
		template <> const char *Tag<TagId::logModule>   ::name = "log module";
		template <> const char *Tag<TagId::mathModule>  ::name = "math module";
		template <> const char *Tag<TagId::physModule>  ::name = "physics module";
		template <> const char *Tag<TagId::resModule>   ::name = "resource module";
		template <> const char *Tag<TagId::scriptModule>::name = "script module";
		template <> const char *Tag<TagId::sysModule>   ::name = "system module";
		template <> const char *Tag<TagId::uiModule>    ::name = "user-interface module";
		template <> const char *Tag<TagId::utilModule>  ::name = "utilities module";
		template <> const char *Tag<TagId::vidModule>   ::name = "video module";

		/*-----------------------+
		| platform-specific tags |
		+-----------------------*/

#ifdef USE_ANGELSCRIPT
		template <> const char *Tag<TagId::angelscriptPlatform>::name = STRINGIZE(ANGELSCRIPT_NAME);
#endif
#ifdef USE_BZIP2
		template <> const char *Tag<TagId::bzip2Platform>      ::name = STRINGIZE(BZIP2_NAME);
#endif
#ifdef USE_CARBON
		template <> const char *Tag<TagId::carbonPlatform>     ::name = STRINGIZE(CARBON_NAME);
#endif
#ifdef USE_CG
		template <> const char *Tag<TagId::cgPlatform>         ::name = STRINGIZE(CG_NAME);
#endif
#ifdef USE_CURL
		template <> const char *Tag<TagId::curlPlatform>       ::name = STRINGIZE(CURL_NAME);
#endif
#ifdef USE_D3D9
		template <> const char *Tag<TagId::d3d9Platform>       ::name = STRINGIZE(D3D9_NAME);
#endif
#ifdef USE_DINPUT9
		template <> const char *Tag<TagId::dinput9Platform>    ::name = STRINGIZE(DINPUT9_NAME);
#endif
#ifdef USE_DSOUND9
		template <> const char *Tag<TagId::dsound9Platform>    ::name = STRINGIZE(DSOUND9_NAME);
#endif
#ifdef USE_EXPAT
		template <> const char *Tag<TagId::expatPlatform>      ::name = STRINGIZE(EXPAT_NAME);
#endif
#ifdef USE_FFMPEG
		template <> const char *Tag<TagId::ffmpegPlatform>     ::name = STRINGIZE(FFMPEG_NAME);
#endif
#ifdef USE_FLAC
		template <> const char *Tag<TagId::flacPlatform>       ::name = STRINGIZE(FLAC_NAME);
#endif
#ifdef USE_FREETYPE
		template <> const char *Tag<TagId::freetypePlatform>   ::name = STRINGIZE(FREETYPE_NAME);
#endif
#ifdef USE_GIF
		template <> const char *Tag<TagId::gifPlatform>        ::name = STRINGIZE(GIF_NAME);
#endif
#ifdef USE_JPEG
		template <> const char *Tag<TagId::jpegPlatform>       ::name = STRINGIZE(JPEG_NAME);
#endif
#ifdef USE_LUA
		template <> const char *Tag<TagId::luaPlatform>        ::name = STRINGIZE(LUA_NAME);
#endif
#ifdef USE_MINIZIP
		template <> const char *Tag<TagId::minizipPlatform>    ::name = STRINGIZE(MINIZIP_NAME);
#endif
#ifdef USE_MNG
		template <> const char *Tag<TagId::mngPlatform>        ::name = STRINGIZE(MNG_NAME);
#endif
#ifdef USE_OGG
		template <> const char *Tag<TagId::oggPlatform>        ::name = STRINGIZE(OGG_NAME);
#endif
#ifdef USE_OPENAL
		template <> const char *Tag<TagId::openalPlatform>     ::name = STRINGIZE(OPENAL_NAME);
#endif
#ifdef USE_OPENEXR
		template <> const char *Tag<TagId::openexrPlatform>    ::name = STRINGIZE(OPENEXR_NAME);
#endif
#ifdef USE_OPENGL
		template <> const char *Tag<TagId::openglPlatform>     ::name = STRINGIZE(OPENGL_NAME);
#endif
#ifdef USE_PNG
		template <> const char *Tag<TagId::pngPlatform>        ::name = STRINGIZE(PNG_NAME);
#endif
#ifdef USE_POSIX
		template <> const char *Tag<TagId::posixPlatform>      ::name = STRINGIZE(POSIX_NAME);
#endif
#ifdef USE_PYTHON
		template <> const char *Tag<TagId::pythonPlatform>     ::name = STRINGIZE(PYTHON_NAME);
#endif
#ifdef USE_SPHINX
		template <> const char *Tag<TagId::sphinxPlatform>     ::name = STRINGIZE(SPHINX_NAME);
#endif
#ifdef USE_SQUIRREL
		template <> const char *Tag<TagId::squirrelPlatform>   ::name = STRINGIZE(SQUIRREL_NAME);
#endif
#ifdef USE_THEORA
		template <> const char *Tag<TagId::theoraPlatform>     ::name = STRINGIZE(THEORA_NAME);
#endif
#ifdef USE_TIFF
		template <> const char *Tag<TagId::tiffPlatform>       ::name = STRINGIZE(TIFF_NAME);
#endif
#ifdef USE_UNGIF
		template <> const char *Tag<TagId::ungifPlatform>      ::name = STRINGIZE(UNGIF_NAME);
#endif
#ifdef USE_VORBIS
		template <> const char *Tag<TagId::vorbisPlatform>     ::name = STRINGIZE(VORBIS_NAME);
#endif
#ifdef USE_WIN32
		template <> const char *Tag<TagId::win32Platform>      ::name = STRINGIZE(WIN32_NAME);
#endif
#ifdef USE_X11
		template <> const char *Tag<TagId::x11Platform>        ::name = STRINGIZE(X11_NAME);
#endif
#ifdef USE_ZIP
		template <> const char *Tag<TagId::zipPlatform>        ::name = STRINGIZE(ZIP_NAME);
#endif
#ifdef USE_ZLIB
		template <> const char *Tag<TagId::zlibPlatform>       ::name = STRINGIZE(ZLIB_NAME);
#endif
	}
}
