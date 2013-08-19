#ifndef    page_local_err_tags_hpp
#   define page_local_err_tags_hpp

#	include <string>

#	include <boost/exception/error_info.hpp>

#	include "Tag.hpp"

namespace page { namespace err
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
			conflict,
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
			errModule,
			gameModule,
			guiModule,
			inpModule,
			logModule,
			mathModule,
			physModule,
			resModule,
			scriptModule,
			sysModule,
			utilModule,
			vidModule,
			wndModule,

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
	 * @class AccessDeniedTag
	 * A tag to be used when access is not permitted to a resource.
	 */
	struct AccessDeniedTag;

	/**
	 * @class AllocTag
	 * A tag to be used when a container runs out of space.
	 */
	struct AllocTag;

	/**
	 * @class CatastrophicTag
	 * A tag to be used when a catastrophic failure occurs.
	 */
	struct CatastrophicTag;

	/**
	 * @class ConflictTag
	 * A tag to be used when a naming conflict occurs.
	 */
	struct ConflictTag;

	/**
	 * @class ConversionTag
	 * A tag to be used when something cannot be converted to something else.
	 */
	struct ConversionTag;

	/**
	 * @class DeliberateTag
	 * A tag to be used when an exception is thrown deliberately.
	 */
	struct DeliberateTag;

	/**
	 * @class EndTag
	 * A tag to be used when the end of something is reached.
	 */
	struct EndTag;

	/**
	 * @class EndOfFileTag
	 * A tag to be used when the end of a file is reached.
	 */
	struct EndOfFileTag;

	/**
	 * @class EndOfStreamTag
	 * A tag to be used when the end of a stream is reached.
	 */
	struct EndOfStreamTag;

	/**
	 * @class FactoryTag
	 * A tag to be used when an error occurs in a factory.
	 */
	struct FactoryTag;

	/**
	 * @class FileTag
	 * A tag to be used when an error occurs during file handling.
	 */
	struct FileTag;

	/**
	 * @class FileAccessTag
	 * A tag to be used when a file cannot be accessed.
	 */
	struct FileAccessTag;

	/**
	 * @class FileFlushTag
	 * A tag to be used when a file cannot be flushed.
	 */
	struct FileFlushTag;

	/**
	 * @class FileNotFoundTag
	 * A tag to be used when a file is not found.
	 */
	struct FileNotFoundTag;

	/**
	 * @class FileOpenTag
	 * A tag to be used when a file cannot be opened.
	 */
	struct FileOpenTag;

	/**
	 * @class FileReadTag
	 * A tag to be used when a file-read operation fails.
	 */
	struct FileReadTag;

	/**
	 * @class FileSeekTag
	 * A tag to be used when a file-seek operation fails.
	 */
	struct FileSeekTag;

	/**
	 * @class FileTellTag
	 * A tag to be used when the current position in a file cannot be
	 * determined.
	 */
	struct FileTellTag;

	/**
	 * @class FileWriteTag
	 * A tag to be used when a file-write operation fails.
	 */
	struct FileWriteTag;

	/**
	 * @class FormatTag
	 * A tag to be used when an error is caused by unexpected formatting of the
	 * input, such as in the case of a corrupt file.
	 */
	struct FormatTag;

	/**
	 * @class InputTag
	 * A tag to be used when an error occurs during an input operation.
	 */
	struct InputTag;

	/**
	 * @class IoTag
	 * A tag for input/output errors.
	 */
	struct IoTag;

	/**
	 * @class KeyTag
	 * A tag for errors having to do with a key.
	 */
	struct KeyTag;

	/**
	 * @class KeyConflictTag
	 * A tag to be used when there is a conflict between two keys in a uniquely-
	 * keyed container.
	 */
	struct KeyConflictTag;

	/**
	 * @class KeyNotFoundTag
	 * A tag to be used when there is no matching key in a container.
	 */
	struct KeyNotFoundTag;

	/**
	 * @class ModuleTag
	 * A tag to be used when an error occurs within a certain module of the
	 * program.
	 *
	 * @note This tag should be used in combination with one or more module-
	 *       specific tags to specify which modules were involved.
	 */
	struct ModuleTag;

	/**
	 * @class NotAvailableTag
	 * A tag to be used when a certain thing is requested, but no more of such
	 * things are available.
	 */
	struct NotAvailableTag;

	/**
	 * @class NotDirectoryTag
	 * A tag to be used when a path does not point to a directory.
	 */
	struct NotDirectoryTag;

	/**
	 * @class NotFileTag
	 * A tag to be used when a path does not point to a file.
	 */
	struct NotFileTag;

	/**
	 * @class NotFoundTag
	 * A tag to be used when something was searched for but couldn't be found.
	 */
	struct NotFoundTag;

	/**
	 * @class OutputTag
	 * A tag to be used when an error occurs during an output operation.
	 */
	struct OutputTag;

	/**
	 * @class ParameterTag
	 * A tag to be used when an error is caused by an invalid parameter.
	 */
	struct ParameterTag;

	/**
	 * @class ParserTag
	 * A tag to be used when an error occurs during parsing.
	 */
	struct ParserTag;

	/**
	 * @class PathTag
	 * A tag to be used when an error involves a filesystem or network path.
	 *
	 * @note Consider adding @c boost::errinfo_file_name to the exception.
	 */
	struct PathTag;

	/**
	 * @class PlatformTag
	 * A tag to be used when an error occurs during the use of a third-party
	 * platform.
	 *
	 * @note This tag should be used in combination with one or more platform-
	 *       specific tags to specify which platforms were involved.
	 */
	struct PlatformTag;

	/**
	 * @class RangeTag
	 * A tag to be used when something goes beyond its specified range.
	 */
	struct RangeTag;

	/**
	 * @class ReadOnlyTag
	 * A tag to be used when an attempt to modify something immutable fails.
	 */
	struct ReadOnlyTag;

	/**
	 * @class SerializationTag
	 * A tag to be used when an attempt to serialize data fails.
	 */
	struct SerializationTag;

	/**
	 * @class StreamTag
	 * A tag to be used when an error occurs during streaming.
	 */
	struct StreamTag;

	/**
	 * @class StreamFlushTag
	 * A tag to be used when a stream cannot be flushed.
	 */
	struct StreamFlushTag;

	/**
	 * @class StreamReadTag
	 * A tag to be used when a stream-read operation fails.
	 */
	struct StreamReadTag;

	/**
	 * @class StreamSeekTag
	 * A tag to be used when a stream-seek operation fails.
	 */
	struct StreamSeekTag;

	/**
	 * @class StreamTellTag
	 * A tag to be used when the current position in a stream cannot be
	 * determined.
	 */
	struct StreamTellTag;

	/**
	 * @class StreamWriteTag
	 * A tag to be used when a stream-write operation fails.
	 */
	struct StreamWriteTag;

	/**
	 * @class TokenTag
	 * A tag to be used when there is an error having to do with a token.
	 */
	struct TokenTag;

	/**
	 * @class TypeTag
	 * A tag to be used when an error occurs having to do with a type.
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
	struct ConflictTag      : Tag<TagId::conflict>      {};
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

	struct KeyConflict      :                           TagSet<KeyTag, ConflictTag>         {};
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
	 * @class AudModuleTag
	 * A tag to be used when an error occurs within the audio module.
	 *
	 * @note The audio module is contained in the @c ::page::aud namespace and
	 *       the @c src/local/aud directory.
	 */
	struct AudModuleTag : Tag<TagId::audModule>, TagSet<ModuleTag> {};

	/**
	 * @class CacheModuleTag
	 * A tag to be used when an error occurs within the cache module.
	 *
	 * @note The cache module is contained in the @c ::page::cache namespace and
	 *       the @c src/local/cache directory.
	 */
	struct CacheModuleTag : Tag<TagId::cacheModule>, TagSet<ModuleTag> {};

	/**
	 * @class CfgModuleTag
	 * A tag to be used when an error occurs within the configuration module.
	 *
	 * @note The configuration module is contained in the @c ::page::cfg
	 *       namespace and the @c src/local/cfg directory.
	 */
	struct CfgModuleTag : Tag<TagId::cfgModule>, TagSet<ModuleTag> {};

	/**
	 * @class ErrModuleTag
	 * A tag to be used when an error occurs within the error module.
	 *
	 * @note The error module is contained in the @c ::page::err namespace and
	 *       the @c src/local/err directory.
	 */
	struct ErrModuleTag : Tag<TagId::errModule>, TagSet<ModuleTag> {};

	/**
	 * @class GameModuleTag
	 * A tag to be used when an error occurs within the game module.
	 *
	 * @note The game module is contained in the @c ::page::game namespace and
	 *       the @c src/local/game directory.
	 */
	struct GameModuleTag : Tag<TagId::gameModule>, TagSet<ModuleTag> {};

	/**
	 * @class GuiModuleTag
	 * A tag to be used when an error occurs within the GUI module.
	 *
	 * @note The GUI module is contained in the @c ::page::gui namespace and the
	 *       @c src/local/ui directory.
	 */
	struct GuiModuleTag : Tag<TagId::guiModule>, TagSet<ModuleTag> {};

	/**
	 * @class InpModuleTag
	 * A tag to be used when an error occurs within the input module.
	 *
	 * @note The input module is contained in the @c ::page::inp namespace and
	 *       the @c src/local/inp directory.
	 */
	struct InpModuleTag : Tag<TagId::inpModule>, TagSet<ModuleTag> {};

	/**
	 * @class LogModuleTag
	 * A tag to be used when an error occurs within the log module.
	 *
	 * @note The log module is contained in the @c ::page::log namespace and
	 *       the @c src/local/log directory.
	 */
	struct LogModuleTag : Tag<TagId::logModule>, TagSet<ModuleTag> {};

	/**
	 * @class MathModuleTag
	 * A tag to be used when an error occurs within the math module.
	 *
	 * @note The math module is contained in the @c ::page::math namespace and
	 *       the @c src/local/math directory.
	 */
	struct MathModuleTag : Tag<TagId::mathModule>, TagSet<ModuleTag> {};

	/**
	 * @class PhysModuleTag
	 * A tag to be used when an error occurs within the physics module.
	 *
	 * @note The physics module is contained in the @c ::page::phys namespace
	 *       and the @c src/local/phys directory.
	 */
	struct PhysModuleTag : Tag<TagId::physModule>, TagSet<ModuleTag> {};

	/**
	 * @class ResModuleTag
	 * A tag to be used when an error occurs within the resource module.
	 *
	 * @note The resource module is contained in the @c ::page::res namespace
	 *       and the @c src/local/res directory.
	 */
	struct ResModuleTag : Tag<TagId::resModule>, TagSet<ModuleTag> {};

	/**
	 * @class ScriptModuleTag
	 * A tag to be used when an error occurs within the script module.
	 *
	 * @note The script module is contained in the @c ::page::script namespace
	 *       and the @c src/local/script directory.
	 */
	struct ScriptModuleTag : Tag<TagId::scriptModule>, TagSet<ModuleTag> {};

	/**
	 * @class SysModuleTag
	 * A tag to be used when an error occurs within the system module.
	 *
	 * @note The system module is contained in the @c ::page::sys namespace and
	 *       the @c src/local/sys directory.
	 */
	struct SysModuleTag : Tag<TagId::sysModule>, TagSet<ModuleTag> {};

	/**
	 * @class UtilModuleTag
	 * A tag to be used when an error occurs within the utility module.
	 *
	 * @note The utility module is contained in the @c ::page::util namespace
	 *       and the @c src/local/util directory.
	 */
	struct UtilModuleTag : Tag<TagId::utilModule>, TagSet<ModuleTag> {};

	/**
	 * @class VidModuleTag
	 * A tag to be used when an error occurs within the video module.
	 *
	 * @note The video module is contained in the @c ::page::vid namespace and
	 *       the @c src/local/vid directory.
	 */
	struct VidModuleTag : Tag<TagId::vidModule>, TagSet<ModuleTag> {};

	/**
	 * @class WndModuleTag
	 * A tag to be used when an error occurs within the windowing module.
	 *
	 * @note The windowing module is contained in the @c ::page::wnd namespace
	 *       and the @c src/local/wnd directory.
	 */
	struct WndModuleTag : Tag<TagId::wndModule>, TagSet<ModuleTag> {};

	/*-----------------------+
	| platform-specific tags |
	+-----------------------*/

#	ifdef USE_ANGELSCRIPT
	/**
	 * @class AngelscriptPlatformTag
	 * A tag to be used when an error occurs during the use of the AngelScript
	 * platform.
	 */
	struct AngelscriptPlatformTag : Tag<TagId::angelscriptPlatform>, TagSet<PlatformTag> {};
#	endif

#	ifdef USE_BZIP2
	/**
	 * @class Bzip2PlatformTag
	 * A tag to be used when an error occurs during the use of the bzip2
	 * platform.
	 */
	struct Bzip2PlatformTag       : Tag<TagId::bzip2Platform>,       TagSet<PlatformTag> {};
#	endif

#	ifdef USE_CARBON
	/**
	 * @class CarbonPlatformTag
	 * A tag to be used when an error occurs during the use of the Carbon
	 * platform.
	 */
	struct CarbonPlatformTag      : Tag<TagId::carbonPlatform>,      TagSet<PlatformTag> {};
#	endif

#	ifdef USE_CG
	/**
	 * @class CgPlatformTag
	 * A tag to be used when an error occurs during the use of Cg platform.
	 */
	struct CgPlatformTag          : Tag<TagId::cgPlatform>,          TagSet<PlatformTag> {};
#	endif

#	ifdef USE_CURL
	/**
	 * @class CurlPlatformTag
	 * A tag to be used when an error occurs during the use of the libcurl
	 * platform.
	 */
	struct CurlPlatformTag        : Tag<TagId::curlPlatform>,        TagSet<PlatformTag> {};
#	endif

#	ifdef USE_D3D9
	/**
	 * @class D3d9PlatformTag
	 * A tag to be used when an error occurs during the use of the Direct3D 9
	 * platform.
	 */
	struct D3d9PlatformTag        : Tag<TagId::d3d9Platform>,        TagSet<PlatformTag> {};
#	endif

#	ifdef USE_DINPUT9
	/**
	 * @class Dinput9PlatformTag
	 * A tag to be used when an error occurs during the use of the DirectInput 9
	 * platform.
	 */
	struct Dinput9PlatformTag     : Tag<TagId::dinput9Platform>,     TagSet<PlatformTag> {};
#	endif

#	ifdef USE_DSOUND9
	/**
	 * @class Dsound9PlatformTag
	 * A tag to be used when an error occurs during the use of the DirectSound 9
	 * platform.
	 */
	struct Dsound9PlatformTag     : Tag<TagId::dsound9Platform>,     TagSet<PlatformTag> {};
#	endif

#	ifdef USE_EXPAT
	/**
	 * @class ExpatPlatformTag
	 * A tag to be used when an error occurs during the use of the Expat
	 * platform.
	 */
	struct ExpatPlatformTag       : Tag<TagId::expatPlatform>,       TagSet<PlatformTag> {};
#	endif

#	ifdef USE_FFMPEG
	/**
	 * @class FfmpegPlatformTag
	 * A tag to be used when an error occurs during the use of the FFmpeg
	 * platform.
	 */
	struct FfmpegPlatformTag      : Tag<TagId::ffmpegPlatform>,      TagSet<PlatformTag> {};
#	endif

#	ifdef USE_FLAC
	/**
	 * @class FlacPlatformTag
	 * A tag to be used when an error occurs during the use of the libFLAC
	 * platform.
	 */
	struct FlacPlatformTag        : Tag<TagId::flacPlatform>,        TagSet<PlatformTag> {};
#	endif

#	ifdef USE_FREETYPE
	/**
	 * @class FreetypePlatformTag
	 * A tag to be used when an error occurs during the use of the FreeType
	 * 2 platform.
	 */
	struct FreetypePlatformTag    : Tag<TagId::freetypePlatform>,    TagSet<PlatformTag> {};
#	endif

#	ifdef USE_GIF
	/**
	 * @class GifPlatformTag
	 * A tag to be used when an error occurs during the use of the giflib
	 * platform.
	 */
	struct GifPlatformTag         : Tag<TagId::gifPlatform>,         TagSet<PlatformTag> {};
#	endif

#	ifdef USE_JPEG
	/**
	 * @class JpegPlatformTag
	 * A tag to be used when an error occurs during the use of the libjpeg
	 * platform.
	 */
	struct JpegPlatformTag        : Tag<TagId::jpegPlatform>,        TagSet<PlatformTag> {};
#	endif

#	ifdef USE_LUA
	/**
	 * @class LuaPlatformTag
	 * A tag to be used when an error occurs during the use of the Lua platform.
	 */
	struct LuaPlatformTag         : Tag<TagId::luaPlatform>,         TagSet<PlatformTag> {};
#	endif

#	ifdef USE_MINIZIP
	/**
	 * @class MinizipPlatformTag
	 * A tag to be used when an error occurs during the use of the Minizip
	 * platform.
	 */
	struct MinizipPlatformTag     : Tag<TagId::minizipPlatform>,     TagSet<PlatformTag> {};
#	endif

#	ifdef USE_MNG
	/**
	 * @class MngPlatformTag
	 * A tag to be used when an error occurs during the use of the libmng
	 * platform.
	 */
	struct MngPlatformTag         : Tag<TagId::mngPlatform>,         TagSet<PlatformTag> {};
#	endif

#	ifdef USE_OGG
	/**
	 * @class OggPlatformTag
	 * A tag to be used when an error occurs during the use of the libogg
	 * platform.
	 */
	struct OggPlatformTag         : Tag<TagId::oggPlatform>,         TagSet<PlatformTag> {};
#	endif

#	ifdef USE_OPENAL
	/**
	 * @class OpenalPlatformTag
	 * A tag to be used when an error occurs during the use of the OpenAL
	 * platform.
	 */
	struct OpenalPlatformTag      : Tag<TagId::openalPlatform>,      TagSet<PlatformTag> {};
#	endif

#	ifdef USE_OPENEXR
	/**
	 * @class OpenexrPlatformTag
	 * A tag to be used when an error occurs during the use of the OpenEXR
	 * platform.
	 */
	struct OpenexrPlatformTag     : Tag<TagId::openexrPlatform>,     TagSet<PlatformTag> {};
#	endif

#	ifdef USE_OPENGL
	/**
	 * @class OpenglPlatformTag
	 * A tag to be used when an error occurs during the use of the OpenGL
	 * platform.
	 */
	struct OpenglPlatformTag      : Tag<TagId::openglPlatform>,      TagSet<PlatformTag> {};
#	endif

#	ifdef USE_PNG
	/**
	 * @class PngPlatformTag
	 * A tag to be used when an error occurs during the use of the libpng
	 * platform.
	 */
	struct PngPlatformTag         : Tag<TagId::pngPlatform>,         TagSet<PlatformTag> {};
#	endif

#	ifdef USE_POSIX
	/**
	 * @class PosixPlatformTag
	 * A tag to be used when an error occurs during the use of the POSIX
	 * platform.
	 */
	struct PosixPlatformTag       : Tag<TagId::posixPlatform>,       TagSet<PlatformTag> {};
#	endif

#	ifdef USE_PYTHON
	/**
	 * @class PythonPlatformTag
	 * A tag to be used when an error occurs during the use of the Python
	 * platform.
	 */
	struct PythonPlatformTag      : Tag<TagId::pythonPlatform>,      TagSet<PlatformTag> {};
#	endif

#	ifdef USE_SPHINX
	/**
	 * @class SphinxPlatformTag
	 * A tag to be used when an error occurs during the use of the Sphinx
	 * platform.
	 */
	struct SphinxPlatformTag      : Tag<TagId::sphinxPlatform>,      TagSet<PlatformTag> {};
#	endif

#	ifdef USE_SQUIRREL
	/**
	 * @class SquirrelPlatformTag
	 * A tag to be used when an error occurs during the use of the Squirrel
	 * platform.
	 */
	struct SquirrelPlatformTag    : Tag<TagId::squirrelPlatform>,    TagSet<PlatformTag> {};
#	endif

#	ifdef USE_THEORA
	/**
	 * @class TheoraPlatformTag
	 * A tag to be used when an error occurs during the use of the libtheora
	 * platform.
	 */
	struct TheoraPlatformTag      : Tag<TagId::theoraPlatform>,      TagSet<PlatformTag> {};
#	endif

#	ifdef USE_TIFF
	/**
	 * @class TiffPlatformTag
	 * A tag to be used when an error occurs during the use of the libtiff
	 * platform.
	 */
	struct TiffPlatformTag        : Tag<TagId::tiffPlatform>,        TagSet<PlatformTag> {};
#	endif

#	ifdef USE_UNGIF
	/**
	 * @class UngifPlatformTag
	 * A tag to be used when an error occurs during the use of the ungif
	 * platform.
	 */
	struct UngifPlatformTag       : Tag<TagId::ungifPlatform>,       TagSet<PlatformTag> {};
#	endif

#	ifdef USE_VORBIS
	/**
	 * @class VorbisPlatformTag
	 * A tag to be used when an error occurs during the use of the libvorbis
	 * platform.
	 */
	struct VorbisPlatformTag      : Tag<TagId::vorbisPlatform>,      TagSet<PlatformTag> {};
#	endif

#	ifdef USE_WIN32
	/**
	 * @class Win32PlatformTag
	 * A tag to be used when an error occurs during the use of the Win32
	 * platform.
	 */
	struct Win32PlatformTag       : Tag<TagId::win32Platform>,       TagSet<PlatformTag> {};
#	endif

#	ifdef USE_X11
	/**
	 * @class X11PlatformTag
	 * A tag to be used when an error occurs during the use of the X11 platform.
	 */
	struct X11PlatformTag         : Tag<TagId::x11Platform>,         TagSet<PlatformTag> {};
#	endif

#	ifdef USE_ZIP
	/**
	 * @class ZipPlatformTag
	 * A tag to be used when an error occurs during the use of the libzip
	 * platform.
	 */
	struct ZipPlatformTag         : Tag<TagId::zipPlatform>,         TagSet<PlatformTag> {};
#	endif

#	ifdef USE_ZLIB
	/**
	 * @class ZlibPlatformTag
	 * A tag to be used when an error occurs during the use of the zlib
	 * platform.
	 */
	struct ZlibPlatformTag        : Tag<TagId::zlibPlatform>,        TagSet<PlatformTag> {};
#	endif

	/*-----------------------+
	| boost::error_info tags |
	+-----------------------*/

	/**
	 * A @c boost::error_info tag containing the token that is the subject of an
	 * exception tagged with @c TokenTag.
	 */
	using errinfo_token = boost::error_info<struct errinfo_token_, std::string>;
}}

#endif
