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

#ifndef    page_local_err_tags_hpp
#   define page_local_err_tags_hpp

#	include <string>

	// Boost
#	include <boost/exception/error_info.hpp>

	// local
#	include "Tag.hpp"

namespace page
{
	namespace err
	{
		/*--------+
		| tag IDs |
		+--------*/

		namespace TagId
		{
			enum
			{
				accessDenied,
				alloc,
				catastrophic,
				conversion,
				deliberate,
				end,
				factory,
				file,
				fileOpen,
				format,
				input,
				io,
				key,
				module,
				notAvailable,
				notDirectory,
				notFile,
				notFound,
				output,
				parameter,
				parser,
				path,
				platform,
				range,
				readOnly,
				serialization,
				stream,
				streamFlush,
				streamSeek,
				streamTell,
				token,
				type,

				/*------------------------+
				| module-specific tag-IDs |
				+------------------------*/

				audModule,
				cacheModule,
				cfgModule,
				clipModule,
				envModule,
				errModule,
				gameModule,
				inpModule,
				logModule,
				mathModule,
				physModule,
				resModule,
				scriptModule,
				sysModule,
				uiModule,
				utilModule,
				vidModule,

				/*--------------------------+
				| platform-specific tag-IDs |
				+--------------------------*/

#	ifdef USE_ANGELSCRIPT
				angelscriptPlatform,
#	endif
#	ifdef USE_BZIP2
				bzip2Platform,
#	endif
#	ifdef USE_CARBON
				carbonPlatform,
#	endif
#	ifdef USE_CG
				cgPlatform,
#	endif
#	ifdef USE_CURL
				curlPlatform,
#	endif
#	ifdef USE_D3D9
				d3d9Platform,
#	endif
#	ifdef USE_DINPUT9
				dinput9Platform,
#	endif
#	ifdef USE_DSOUND9
				dsound9Platform,
#	endif
#	ifdef USE_EXPAT
				expatPlatform,
#	endif
#	ifdef USE_FFMPEG
				ffmpegPlatform,
#	endif
#	ifdef USE_FLAC
				flacPlatform,
#	endif
#	ifdef USE_FREETYPE
				freetypePlatform,
#	endif
#	ifdef USE_GIF
				gifPlatform,
#	endif
#	ifdef USE_JPEG
				jpegPlatform,
#	endif
#	ifdef USE_LUA
				luaPlatform,
#	endif
#	ifdef USE_MINIZIP
				minizipPlatform,
#	endif
#	ifdef USE_MNG
				mngPlatform,
#	endif
#	ifdef USE_OGG
				oggPlatform,
#	endif
#	ifdef USE_OPENAL
				openalPlatform,
#	endif
#	ifdef USE_OPENEXR
				openexrPlatform,
#	endif
#	ifdef USE_OPENGL
				openglPlatform,
#	endif
#	ifdef USE_PNG
				pngPlatform,
#	endif
#	ifdef USE_POSIX
				posixPlatform,
#	endif
#	ifdef USE_PYTHON
				pythonPlatform,
#	endif
#	ifdef USE_SPHINX
				sphinxPlatform,
#	endif
#	ifdef USE_SQUIRREL
				squirrelPlatform,
#	endif
#	ifdef USE_THEORA
				theoraPlatform,
#	endif
#	ifdef USE_TIFF
				tiffPlatform,
#	endif
#	ifdef USE_UNGIF
				ungifPlatform,
#	endif
#	ifdef USE_VORBIS
				vorbisPlatform,
#	endif
#	ifdef USE_WIN32
				win32Platform,
#	endif
#	ifdef USE_X11
				x11Platform,
#	endif
#	ifdef USE_ZIP
				zipPlatform,
#	endif
#	ifdef USE_ZLIB
				zlibPlatform,
#	endif
				user
			};
		}

		/*-------------------------+
		| tag forward-declarations |
		+-------------------------*/

		/**
		 *
		 */
		struct AccessDeniedTag;

		/**
		 *
		 */
		struct AllocTag;

		/**
		 *
		 */
		struct CatastrophicTag;

		/**
		 *
		 */
		struct ConversionTag;

		/**
		 *
		 */
		struct DeliberateTag;

		/**
		 *
		 */
		struct EndTag;

		/**
		 *
		 */
		struct EndOfFileTag;

		/**
		 *
		 */
		struct EndOfStreamTag;

		/**
		 *
		 */
		struct FactoryTag;

		/**
		 *
		 */
		struct FileTag;

		/**
		 *
		 */
		struct FileAccessTag;

		/**
		 *
		 */
		struct FileFlushTag;

		/**
		 *
		 */
		struct FileNotFoundTag;

		/**
		 *
		 */
		struct FileOpenTag;

		/**
		 *
		 */
		struct FileReadTag;

		/**
		 *
		 */
		struct FileSeekTag;

		/**
		 *
		 */
		struct FileTellTag;

		/**
		 *
		 */
		struct FileWriteTag;

		/**
		 * A tag to be used when an error is caused by unexpected formatting of
		 * the input, such as in the case of a corrupt file.
		 */
		struct FormatTag;

		/**
		 * A tag to be used when an error occurs during an input operation.
		 */
		struct InputTag;

		/**
		 * A tag for input/output errors.
		 */
		struct IoTag;

		/**
		 * A tag for errors having to do with a key.
		 */
		struct KeyTag;

		/**
		 *
		 */
		struct KeyNotFoundTag;

		/**
		 * A tag to be used when an error occurs within a certain module of the
		 * program.
		 *
		 * @note This tag should be used in combination with one or more
		 *       module-specific tags to specify which modules were involved.
		 */
		struct ModuleTag;

		/**
		 * A tag to be used when a certain thing is requested, but no more of
		 * such things are available.
		 */
		struct NotAvailableTag;

		/**
		 *
		 */
		struct NotDirectoryTag;

		/**
		 *
		 */
		struct NotFileTag;

		/**
		 * A tag to be used when something was searched for but couldn't be
		 * found.
		 */
		struct NotFoundTag;

		/**
		 * A tag to be used when an error occurs during an output operation.
		 */
		struct OutputTag;

		/**
		 * A tag to be used when an error is caused by an invalid parameter.
		 */
		struct ParameterTag;

		/**
		 * A tag to be used when an error occurs during parsing.
		 */
		struct ParserTag;

		/**
		 * A tag to be used when an error involves a filesystem or network path.
		 *
		 * @note Consider adding @c boost::errinfo_file_name to the exception.
		 */
		struct PathTag;

		/**
		 * A tag to be used when an error occurs during the use of a third-party
		 * platform.
		 *
		 * @note This tag should be used in combination with one or more
		 *       platform-specific tags to specify which platforms were
		 *       involved.
		 */
		struct PlatformTag;

		/**
		 * A tag to be used when something goes beyond its specified range.
		 */
		struct RangeTag;

		/**
		 *
		 */
		struct ReadOnlyTag;

		/**
		 *
		 */
		struct SerializationTag;

		/**
		 *
		 */
		struct StreamTag;

		/**
		 *
		 */
		struct StreamFlushTag;

		/**
		 *
		 */
		struct StreamReadTag;

		/**
		 *
		 */
		struct StreamSeekTag;

		/**
		 *
		 */
		struct StreamTellTag;

		/**
		 *
		 */
		struct StreamWriteTag;

		/**
		 *
		 */
		struct TokenTag;

		/**
		 *
		 */
		struct TypeTag;

		/*------------+
		| tag aliases |
		+------------*/

		/**
		 * @copydoc InputTag
		 */
		using ReadTag = InputTag;

		/**
		 * @copydoc OutputTag
		 */
		using WriteTag = OutputTag;

		/*----------------+
		| tag definitions |
		+----------------*/

		struct AccessDeniedTag  : Tag<TagId::accessDenied>  {};
		struct AllocTag         : Tag<TagId::alloc>         {};
		struct CatastrophicTag  : Tag<TagId::catastrophic>  {};
		struct ConversionTag    : Tag<TagId::conversion>    {};
		struct DeliberateTag    : Tag<TagId::deliberate>    {};
		struct EndTag           : Tag<TagId::end>           {};
		struct FactoryTag       : Tag<TagId::factory>       {};
		struct FileTag          : Tag<TagId::file>          {};
		struct FormatTag        : Tag<TagId::format>        {};
		struct IoTag            : Tag<TagId::io>            {};
		struct KeyTag           : Tag<TagId::key>           {};
		struct ModuleTag        : Tag<TagId::module>        {};
		struct NotAvailableTag  : Tag<TagId::notAvailable>  {};
		struct NotFoundTag      : Tag<TagId::notFound>      {};
		struct ParameterTag     : Tag<TagId::parameter>     {};
		struct ParserTag        : Tag<TagId::parser>        {};
		struct PathTag          : Tag<TagId::path>          {};
		struct PlatformTag      : Tag<TagId::platform>      {};
		struct RangeTag         : Tag<TagId::range>         {};
		struct SerializationTag : Tag<TagId::serialization> {};
		struct StreamTag        : Tag<TagId::stream>        {};
		struct TypeTag          : Tag<TagId::type>          {};

		struct KeyNotFoundTag   :                           TagSet<KeyTag, NotFoundTag>          {};
		struct TokenTag         : Tag<TagId::token>,        TagSet<ParserTag>                    {};

		struct InputTag         : Tag<TagId::input>,        TagSet<IoTag>                        {};
		struct OutputTag        : Tag<TagId::output>,       TagSet<IoTag>                        {};
		struct ReadOnlyTag      : Tag<TagId::readOnly>,     TagSet<WriteTag>                     {};

		struct StreamReadTag    :                           TagSet<StreamTag, ReadTag>           {};
		struct StreamWriteTag   :                           TagSet<StreamTag, WriteTag>          {};
		struct EndOfStreamTag   :                           TagSet<StreamReadTag, EndTag>        {};
		struct StreamFlushTag   : Tag<TagId::streamFlush>,  TagSet<StreamWriteTag>               {};
		struct StreamSeekTag    : Tag<TagId::streamSeek>,   TagSet<StreamReadTag>                {};
		struct StreamTellTag    : Tag<TagId::streamTell>,   TagSet<StreamReadTag>                {};

		struct EndOfFileTag     :                           TagSet<FileTag, EndOfStreamTag>      {};
		struct FileFlushTag     :                           TagSet<FileTag, StreamFlushTag>      {};
		struct FileReadTag      :                           TagSet<FileTag, StreamReadTag>       {};
		struct FileSeekTag      :                           TagSet<FileTag, StreamSeekTag>       {};
		struct FileTellTag      :                           TagSet<FileTag, StreamTellTag>       {};
		struct FileWriteTag     :                           TagSet<FileTag, StreamWriteTag>      {};
		struct NotDirectoryTag  : Tag<TagId::notDirectory>, TagSet<FileTag, TypeTag>             {};
		struct NotFileTag       : Tag<TagId::notFile>,      TagSet<FileTag, TypeTag>             {};

		struct FileOpenTag      : Tag<TagId::fileOpen>,     TagSet<FileTag>                      {};
		struct FileNotFoundTag  :                           TagSet<FileOpenTag, NotFoundTag>     {};
		struct FileAccessTag    :                           TagSet<FileOpenTag, AccessDeniedTag> {};

		/*---------------------+
		| module-specific tags |
		+---------------------*/

		/**
		 * A tag to be used when an error occurs within the audio module.
		 *
		 * @note The audio module is contained in the @c ::page::aud namespace
		 *       and the @c src/local/aud directory.
		 */
		struct AudModuleTag : Tag<TagId::audModule>, TagSet<ModuleTag> {};

		/**
		 * A tag to be used when an error occurs within the cache module.
		 *
		 * @note The cache module is contained in the @c ::page::cache
		 *       namespace and the @c src/local/cache directory.
		 */
		struct CacheModuleTag : Tag<TagId::cacheModule>, TagSet<ModuleTag> {};

		/**
		 * A tag to be used when an error occurs within the configuration
		 * module.
		 *
		 * @note The configuration module is contained in the @c ::page::cfg
		 *       namespace and the @c src/local/cfg directory.
		 */
		struct CfgModuleTag : Tag<TagId::cfgModule>, TagSet<ModuleTag> {};

		/**
		 * A tag to be used when an error occurs within the clip module.
		 *
		 * @note The clip module is contained in the @c ::page::clip namespace
		 *       and the @c src/local/clip directory.
		 */
		struct ClipModuleTag : Tag<TagId::clipModule>, TagSet<ModuleTag> {};

		/**
		 * A tag to be used when an error occurs within the environment module.
		 *
		 * @note The environment module is contained in the @c ::page::env
		 *       namespace and the @c src/local/env directory.
		 */
		struct EnvModuleTag : Tag<TagId::envModule>, TagSet<ModuleTag> {};

		/**
		 * A tag to be used when an error occurs within the error module.
		 *
		 * @note The error module is contained in the @c ::page::err namespace
		 *       and the @c src/local/err directory.
		 */
		struct ErrModuleTag : Tag<TagId::errModule>, TagSet<ModuleTag> {};

		/**
		 * A tag to be used when an error occurs within the game module.
		 *
		 * @note The game module is contained in the @c ::page::game namespace
		 *       and the @c src/local/game directory.
		 */
		struct GameModuleTag : Tag<TagId::gameModule>, TagSet<ModuleTag> {};

		/**
		 * A tag to be used when an error occurs within the input module.
		 *
		 * @note The input module is contained in the @c ::page::inp namespace
		 *       and the @c src/local/inp directory.
		 */
		struct InpModuleTag : Tag<TagId::inpModule>, TagSet<ModuleTag> {};

		/**
		 * A tag to be used when an error occurs within the log module.
		 *
		 * @note The log module is contained in the @c ::page::log namespace
		 *       and the @c src/local/log directory.
		 */
		struct LogModuleTag : Tag<TagId::logModule>, TagSet<ModuleTag> {};

		/**
		 * A tag to be used when an error occurs within the math module.
		 *
		 * @note The math module is contained in the @c ::page::math namespace
		 *       and the @c src/local/math directory.
		 */
		struct MathModuleTag : Tag<TagId::mathModule>, TagSet<ModuleTag> {};

		/**
		 * A tag to be used when an error occurs within the physics module.
		 *
		 * @note The physics module is contained in the @c ::page::phys
		 *       namespace and the @c src/local/phys directory.
		 */
		struct PhysModuleTag : Tag<TagId::physModule>, TagSet<ModuleTag> {};

		/**
		 * A tag to be used when an error occurs within the resource module.
		 *
		 * @note The resource module is contained in the @c ::page::res
		 *       namespace and the @c src/local/res directory.
		 */
		struct ResModuleTag : Tag<TagId::resModule>, TagSet<ModuleTag> {};

		/**
		 * A tag to be used when an error occurs within the script module.
		 *
		 * @note The script module is contained in the @c ::page::script
		 *       namespace and the @c src/local/script directory.
		 */
		struct ScriptModuleTag : Tag<TagId::scriptModule>, TagSet<ModuleTag> {};

		/**
		 * A tag to be used when an error occurs within the system module.
		 *
		 * @note The system module is contained in the @c ::page::sys namespace
		 *       and the @c src/local/sys directory.
		 */
		struct SysModuleTag : Tag<TagId::sysModule>, TagSet<ModuleTag> {};

		/**
		 * A tag to be used when an error occurs within the user-interface
		 * module.
		 *
		 * @note The user-interface module is contained in the @c ::page::ui
		 *       namespace and the @c src/local/ui directory.
		 */
		struct UiModuleTag : Tag<TagId::uiModule>, TagSet<ModuleTag> {};

		/**
		 * A tag to be used when an error occurs within the utilities module.
		 *
		 * @note The utilities module is contained in the @c ::page::util
		 *       namespace and the @c src/local/util directory.
		 */
		struct UtilModuleTag : Tag<TagId::utilModule>, TagSet<ModuleTag> {};

		/**
		 * A tag to be used when an error occurs within the video module.
		 *
		 * @note The video module is contained in the @c ::page::vid namespace
		 *       and the @c src/local/vid directory.
		 */
		struct VidModuleTag : Tag<TagId::vidModule>, TagSet<ModuleTag> {};

		/*-----------------------+
		| platform-specific tags |
		+-----------------------*/

#	ifdef USE_ANGELSCRIPT
		/**
		 * A tag to be used when an error occurs during the use of the
		 * AngelScript platform.
		 */
		struct AngelscriptPlatformTag : Tag<TagId::angelscriptPlatform>, TagSet<PlatformTag> {};
#	endif

#	ifdef USE_BZIP2
		/**
		 * A tag to be used when an error occurs during the use of the bzip2
		 * platform.
		 */
		struct Bzip2PlatformTag       : Tag<TagId::bzip2Platform>,       TagSet<PlatformTag> {};
#	endif

#	ifdef USE_CARBON
		/**
		 * A tag to be used when an error occurs during the use of the Carbon
		 * platform.
		 */
		struct CarbonPlatformTag      : Tag<TagId::carbonPlatform>,      TagSet<PlatformTag> {};
#	endif

#	ifdef USE_CG
		/**
		 * A tag to be used when an error occurs during the use of Cg platform.
		 */
		struct CgPlatformTag          : Tag<TagId::cgPlatform>,          TagSet<PlatformTag> {};
#	endif

#	ifdef USE_CURL
		/**
		 * A tag to be used when an error occurs during the use of the libcurl
		 * platform.
		 */
		struct CurlPlatformTag        : Tag<TagId::curlPlatform>,        TagSet<PlatformTag> {};
#	endif

#	ifdef USE_D3D9
		/**
		 * A tag to be used when an error occurs during the use of the Direct3D
		 * 9 platform.
		 */
		struct D3d9PlatformTag        : Tag<TagId::d3d9Platform>,        TagSet<PlatformTag> {};
#	endif

#	ifdef USE_DINPUT9
		/**
		 * A tag to be used when an error occurs during the use of the
		 * DirectInput 9 platform.
		 */
		struct Dinput9PlatformTag     : Tag<TagId::dinput9Platform>,     TagSet<PlatformTag> {};
#	endif

#	ifdef USE_DSOUND9
		/**
		 * A tag to be used when an error occurs during the use of the
		 * DirectSound 9 platform.
		 */
		struct Dsound9PlatformTag     : Tag<TagId::dsound9Platform>,     TagSet<PlatformTag> {};
#	endif

#	ifdef USE_EXPAT
		/**
		 * A tag to be used when an error occurs during the use of the Expat
		 * platform.
		 */
		struct ExpatPlatformTag       : Tag<TagId::expatPlatform>,       TagSet<PlatformTag> {};
#	endif

#	ifdef USE_FFMPEG
		/**
		 * A tag to be used when an error occurs during the use of the FFmpeg
		 * platform.
		 */
		struct FfmpegPlatformTag      : Tag<TagId::ffmpegPlatform>,      TagSet<PlatformTag> {};
#	endif

#	ifdef USE_FLAC
		/**
		 * A tag to be used when an error occurs during the use of the libFLAC
		 * platform.
		 */
		struct FlacPlatformTag        : Tag<TagId::flacPlatform>,        TagSet<PlatformTag> {};
#	endif

#	ifdef USE_FREETYPE
		/**
		 * A tag to be used when an error occurs during the use of the FreeType
		 * 2 platform.
		 */
		struct FreetypePlatformTag    : Tag<TagId::freetypePlatform>,    TagSet<PlatformTag> {};
#	endif

#	ifdef USE_GIF
		/**
		 * A tag to be used when an error occurs during the use of the giflib
		 * platform.
		 */
		struct GifPlatformTag         : Tag<TagId::gifPlatform>,         TagSet<PlatformTag> {};
#	endif

#	ifdef USE_JPEG
		/**
		 * A tag to be used when an error occurs during the use of the libjpeg
		 * platform.
		 */
		struct JpegPlatformTag        : Tag<TagId::jpegPlatform>,        TagSet<PlatformTag> {};
#	endif

#	ifdef USE_LUA
		/**
		 * A tag to be used when an error occurs during the use of the Lua
		 * platform.
		 */
		struct LuaPlatformTag         : Tag<TagId::luaPlatform>,         TagSet<PlatformTag> {};
#	endif

#	ifdef USE_MINIZIP
		/**
		 * A tag to be used when an error occurs during the use of the Minizip
		 * platform.
		 */
		struct MinizipPlatformTag     : Tag<TagId::minizipPlatform>,     TagSet<PlatformTag> {};
#	endif

#	ifdef USE_MNG
		/**
		 * A tag to be used when an error occurs during the use of the libmng
		 * platform.
		 */
		struct MngPlatformTag         : Tag<TagId::mngPlatform>,         TagSet<PlatformTag> {};
#	endif

#	ifdef USE_OGG
		/**
		 * A tag to be used when an error occurs during the use of the libogg
		 * platform.
		 */
		struct OggPlatformTag         : Tag<TagId::oggPlatform>,         TagSet<PlatformTag> {};
#	endif

#	ifdef USE_OPENAL
		/**
		 * A tag to be used when an error occurs during the use of the OpenAL
		 * platform.
		 */
		struct OpenalPlatformTag      : Tag<TagId::openalPlatform>,      TagSet<PlatformTag> {};
#	endif

#	ifdef USE_OPENEXR
		/**
		 * A tag to be used when an error occurs during the use of the OpenEXR
		 * platform.
		 */
		struct OpenexrPlatformTag     : Tag<TagId::openexrPlatform>,     TagSet<PlatformTag> {};
#	endif

#	ifdef USE_OPENGL
		/**
		 * A tag to be used when an error occurs during the use of the OpenGL
		 * platform.
		 */
		struct OpenglPlatformTag      : Tag<TagId::openglPlatform>,      TagSet<PlatformTag> {};
#	endif

#	ifdef USE_PNG
		/**
		 * A tag to be used when an error occurs during the use of the libpng
		 * platform.
		 */
		struct PngPlatformTag         : Tag<TagId::pngPlatform>,         TagSet<PlatformTag> {};
#	endif

#	ifdef USE_POSIX
		/**
		 * A tag to be used when an error occurs during the use of the POSIX
		 * platform.
		 */
		struct PosixPlatformTag       : Tag<TagId::posixPlatform>,       TagSet<PlatformTag> {};
#	endif

#	ifdef USE_PYTHON
		/**
		 * A tag to be used when an error occurs during the use of the Python
		 * platform.
		 */
		struct PythonPlatformTag      : Tag<TagId::pythonPlatform>,      TagSet<PlatformTag> {};
#	endif

#	ifdef USE_SPHINX
		/**
		 * A tag to be used when an error occurs during the use of the Sphinx
		 * platform.
		 */
		struct SphinxPlatformTag      : Tag<TagId::sphinxPlatform>,      TagSet<PlatformTag> {};
#	endif

#	ifdef USE_SQUIRREL
		/**
		 * A tag to be used when an error occurs during the use of the Squirrel
		 * platform.
		 */
		struct SquirrelPlatformTag    : Tag<TagId::squirrelPlatform>,    TagSet<PlatformTag> {};
#	endif

#	ifdef USE_THEORA
		/**
		 * A tag to be used when an error occurs during the use of the libtheora
		 * platform.
		 */
		struct TheoraPlatformTag      : Tag<TagId::theoraPlatform>,      TagSet<PlatformTag> {};
#	endif

#	ifdef USE_TIFF
		/**
		 * A tag to be used when an error occurs during the use of the libtiff
		 * platform.
		 */
		struct TiffPlatformTag        : Tag<TagId::tiffPlatform>,        TagSet<PlatformTag> {};
#	endif

#	ifdef USE_UNGIF
		/**
		 * A tag to be used when an error occurs during the use of the ungif
		 * platform.
		 */
		struct UngifPlatformTag       : Tag<TagId::ungifPlatform>,       TagSet<PlatformTag> {};
#	endif

#	ifdef USE_VORBIS
		/**
		 * A tag to be used when an error occurs during the use of the libvorbis
		 * platform.
		 */
		struct VorbisPlatformTag      : Tag<TagId::vorbisPlatform>,      TagSet<PlatformTag> {};
#	endif

#	ifdef USE_WIN32
		/**
		 * A tag to be used when an error occurs during the use of the Win32
		 * platform.
		 */
		struct Win32PlatformTag       : Tag<TagId::win32Platform>,       TagSet<PlatformTag> {};
#	endif

#	ifdef USE_X11
		/**
		 * A tag to be used when an error occurs during the use of the X11
		 * platform.
		 */
		struct X11PlatformTag         : Tag<TagId::x11Platform>,         TagSet<PlatformTag> {};
#	endif

#	ifdef USE_ZIP
		/**
		 * A tag to be used when an error occurs during the use of the libzip
		 * platform.
		 */
		struct ZipPlatformTag         : Tag<TagId::zipPlatform>,         TagSet<PlatformTag> {};
#	endif

#	ifdef USE_ZLIB
		/**
		 * A tag to be used when an error occurs during the use of the zlib
		 * platform.
		 */
		struct ZlibPlatformTag        : Tag<TagId::zlibPlatform>,        TagSet<PlatformTag> {};
#	endif

		/*-----------------------+
		| boost::error_info tags |
		+-----------------------*/

		/**
		 * A @c boost::error_info tag containing the token that is the subject
		 * of an exception tagged with @c TokenTag.
		 */
		using errinfo_token = boost::error_info<struct errinfo_token_, std::string>;
	}
}

#endif
