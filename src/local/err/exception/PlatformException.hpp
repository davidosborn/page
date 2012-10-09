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

#ifndef    page_local_err_exception_PlatformException_hpp
#   define page_local_err_exception_PlatformException_hpp

#	include "../../util/string.hpp" // Join, StartsWith
#	include "Exception.hpp"
#	include "tags.hpp" // TagId

#	ifdef USE_OPENAL
#		include "../openal.hpp" // ResetError
#	endif
#	ifdef USE_OPENGL
#		include "../opengl.hpp" // ResetError
#	endif
#	ifdef USE_POSIX
#		include <cerrno>
#		include <cstring> // strerror
#		include "../posix.hpp" // ResetError
#	endif

namespace page
{
	namespace err
	{
		namespace detail
		{
			template <TagId id> struct PlatformName;
			template <TagId id> struct PlatformWhat
			{
				static std::string Value()
				{
					return "error";
				}
			};
			template <TagId id> struct PlatformReset
			{
				static void Do() {}
			};

#	define PLATFORM_NAME(NAME, STRING) \
		template <> struct PlatformName<TagId::NAME##Platform> \
		{ \
			static std::string Value() \
			{ \
				return STRING; \
			} \
		};
#	define PLATFORM_WHAT(NAME, WHAT) \
		template <> struct PlatformWhat<TagId::NAME##Platform> \
		{ \
			static std::string Value() \
			{ \
				return WHAT; \
			} \
		};
#	define PLATFORM_RESET(NAME, RESET) \
		template <> struct PlatformReset<TagId::NAME##Platform> \
		{ \
			static void Do() \
			{ \
				return RESET; \
			} \
		};

#	ifdef USE_ANGELSCRIPT
			PLATFORM_NAME(Angelscript, "AngelScript")
#	endif
#	ifdef USE_BZIP2
			PLATFORM_NAME(Bzip2, "libbzip2")
#	endif
#	ifdef USE_CARBON
			PLATFORM_NAME(Carbon, "Carbon")
#	endif
#	ifdef USE_CG
			PLATFORM_NAME(Cg, "Cg")
#	endif
#	ifdef USE_CURL
			PLATFORM_NAME(Curl, "libcurl")
#	endif
#	ifdef USE_D3D9
			PLATFORM_NAME(D3d9, "Direct3D 9")
#	endif
#	ifdef USE_DINPUT9
			PLATFORM_NAME(Dinput9, "DirectInput 9")
#	endif
#	ifdef USE_DSOUND9
			PLATFORM_NAME(Dsound9, "DirectSound 9")
#	endif
#	ifdef USE_EXPAT
			PLATFORM_NAME(Expat, "Expat")
#	endif
#	ifdef USE_FFMPEG
			PLATFORM_NAME(Ffmpeg, "FFmpeg")
#	endif
#	ifdef USE_FLAC
			PLATFORM_NAME(Flac, "libFLAC")
#	endif
#	ifdef USE_FREETYPE
			PLATFORM_NAME(Freetype, "FreeType")
#	endif
#	ifdef USE_GIF
			PLATFORM_NAME(Gif, "giflib")
#	endif
#	ifdef USE_JPEG
			PLATFORM_NAME(Jpeg, "libjpeg")
#	endif
#	ifdef USE_LUA
			PLATFORM_NAME(Lua, "Lua")
#	endif
#	ifdef USE_MINIZIP
			PLATFORM_NAME(Minizip, "minizip")
#	endif
#	ifdef USE_MNG
			PLATFORM_NAME(Mng, "libmng")
#	endif
#	ifdef USE_OGG
			PLATFORM_NAME(Ogg, "libogg")
#	endif
#	ifdef USE_OPENAL
			PLATFORM_NAME(Openal, "OpenAL")
			PLATFORM_RESET(Openal, openal::ResetError())
#	endif
#	ifdef USE_OPENEXR
			PLATFORM_NAME(Openexr, "OpenEXR")
#	endif
#	ifdef USE_OPENGL
			PLATFORM_NAME(Opengl, "OpenGL")
			PLATFORM_RESET(Opengl, opengl::ResetError())
#	endif
#	ifdef USE_PNG
			PLATFORM_NAME(Png, "libpng")
#	endif
#	ifdef USE_POSIX
			PLATFORM_NAME(Posix, "POSIX")
			PLATFORM_WHAT(Posix, ::strerror(errno)) // HACK: fails with ::errno
			PLATFORM_RESET(Posix, posix::ResetError())
#	endif
#	ifdef USE_PYTHON
			PLATFORM_NAME(Python, "Python")
#	endif
#	ifdef USE_SPHINX
			PLATFORM_NAME(Sphinx, "Sphinx")
#	endif
#	ifdef USE_SQUIRREL
			PLATFORM_NAME(Squirrel, "Squirrel")
#	endif
#	ifdef USE_THEORA
			PLATFORM_NAME(Theora, "libtheora")
#	endif
#	ifdef USE_TIFF
			PLATFORM_NAME(Tiff, "libtiff")
#	endif
#	ifdef USE_UNGIF
			PLATFORM_NAME(Ungif, "ungif")
#	endif
#	ifdef USE_VORBIS
			PLATFORM_NAME(Vorbis, "libvorbis")
#	endif
#	ifdef USE_WIN32
			PLATFORM_NAME(Win32, "Win32")
#	endif
#	ifdef USE_X11
			PLATFORM_NAME(X11, "X11")
#	endif
#	ifdef USE_ZIP
			PLATFORM_NAME(Zip, "libzip")
#	endif
#	ifdef USE_ZLIB
			PLATFORM_NAME(Zlib, "zlib")
#	endif
		}

		/**
		 * A tagged exception class for platform-specific errors.
		 *
		 * The platform name is included as part of the error message.
		 */
		template <typename PlatformTag, typename... Tags>
			struct PlatformException : Exception<PlatformTag, err::PlatformTag, Tags...>
		{
			typedef Exception<PlatformTag, err::PlatformTag, Tags...> Base;

			typedef detail::PlatformName <static_cast<TagId>(PlatformTag::id)> Name;
			typedef detail::PlatformWhat <static_cast<TagId>(PlatformTag::id)> What;
			typedef detail::PlatformReset<static_cast<TagId>(PlatformTag::id)> Reset;

			PlatformException() :
				Base(util::Join(std::string("123"), ": ", std::string("ABC")))
			{
				Reset::Do();
			}
			explicit PlatformException(const std::string &what) :
				Base(util::StartsWith(what, Name::Value()) ? what : util::Join(Name::Value(), ": ", what))
			{
				Reset::Do();
			}
		};
	}
}

#endif
