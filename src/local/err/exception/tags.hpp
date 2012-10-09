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

#ifndef    page_local_err_exception_tags_hpp
#   define page_local_err_exception_tags_hpp

namespace page
{
	namespace err
	{
		enum class TagId : int
		{
			AccessDenied,
			Alloc,
			Catastrophic,
			Conversion,
			Deliberate,
			Deserialization,
			EndOfStream,
			File,
			Format,
			Key,
			NotAvailable,
			NotFound,
			Parameter,
			Path,
			Platform,
			Range,
			Read,
			Resource,
			Serialization,
			Stream,
			Type,
			Write,

#	ifdef USE_ANGELSCRIPT
			AngelscriptPlatform,
#	endif
#	ifdef USE_BZIP2
			Bzip2Platform,
#	endif
#	ifdef USE_CARBON
			CarbonPlatform,
#	endif
#	ifdef USE_CG
			CgPlatform,
#	endif
#	ifdef USE_CURL
			CurlPlatform,
#	endif
#	ifdef USE_D3D9
			D3d9Platform,
#	endif
#	ifdef USE_DINPUT9
			Dinput9Platform,
#	endif
#	ifdef USE_DSOUND9
			Dsound9Platform,
#	endif
#	ifdef USE_EXPAT
			ExpatPlatform,
#	endif
#	ifdef USE_FFMPEG
			FfmpegPlatform,
#	endif
#	ifdef USE_FLAC
			FlacPlatform,
#	endif
#	ifdef USE_FREETYPE
			FreetypePlatform,
#	endif
#	ifdef USE_GIF
			GifPlatform,
#	endif
#	ifdef USE_JPEG
			JpegPlatform,
#	endif
#	ifdef USE_LUA
			LuaPlatform,
#	endif
#	ifdef USE_MINIZIP
			MinizipPlatform,
#	endif
#	ifdef USE_MNG
			MngPlatform,
#	endif
#	ifdef USE_OGG
			OggPlatform,
#	endif
#	ifdef USE_OPENAL
			OpenalPlatform,
#	endif
#	ifdef USE_OPENEXR
			OpenexrPlatform,
#	endif
#	ifdef USE_OPENGL
			OpenglPlatform,
#	endif
#	ifdef USE_PNG
			PngPlatform,
#	endif
#	ifdef USE_POSIX
			PosixPlatform,
#	endif
#	ifdef USE_PYTHON
			PythonPlatform,
#	endif
#	ifdef USE_SPHINX
			SphinxPlatform,
#	endif
#	ifdef USE_SQUIRREL
			SquirrelPlatform,
#	endif
#	ifdef USE_THEORA
			TheoraPlatform,
#	endif
#	ifdef USE_TIFF
			TiffPlatform,
#	endif
#	ifdef USE_UNGIF
			UngifPlatform,
#	endif
#	ifdef USE_VORBIS
			VorbisPlatform,
#	endif
#	ifdef USE_WIN32
			Win32Platform,
#	endif
#	ifdef USE_X11
			X11Platform,
#	endif
#	ifdef USE_ZIP
			ZipPlatform,
#	endif
#	ifdef USE_ZLIB
			ZlibPlatform,
#	endif
			END // avoids trailing comma (GCC doesn't support it for C++0x yet)
		};

		template <TagId id_> struct Tag
		{
			enum { id = static_cast<int>(id_) };
		};
		
		/**
		 * Instantiates @c Tag with an arbitrary integral @c id.
		 */
		template <int i> struct ArbitraryTag
		{
			typedef Tag<static_cast<TagId>(i)> type;
		};

		typedef Tag<TagId::AccessDenied>    AccessDeniedTag;
		typedef Tag<TagId::Alloc>           AllocTag;
		typedef Tag<TagId::Catastrophic>    CatastrophicTag;
		typedef Tag<TagId::Conversion>      ConversionTag;
		typedef Tag<TagId::Deliberate>      DeliberateTag;
		typedef Tag<TagId::Deserialization> DeserializationTag;
		typedef Tag<TagId::EndOfStream>     EndOfStreamTag;
		typedef Tag<TagId::File>            FileTag;
		typedef Tag<TagId::Format>          FormatTag;
		typedef Tag<TagId::Key>             KeyTag;
		typedef Tag<TagId::NotAvailable>    NotAvailableTag;
		typedef Tag<TagId::NotFound>        NotFoundTag;
		typedef Tag<TagId::Parameter>       ParameterTag;
		typedef Tag<TagId::Path>            PathTag;
		typedef Tag<TagId::Platform>        PlatformTag;
		typedef Tag<TagId::Range>           RangeTag;
		typedef Tag<TagId::Read>            ReadTag;
		typedef Tag<TagId::Resource>        ResourceTag;
		typedef Tag<TagId::Serialization>   SerializationTag;
		typedef Tag<TagId::Stream>          StreamTag;
		typedef Tag<TagId::Type>            TypeTag;
		typedef Tag<TagId::Write>           WriteTag;

#	ifdef USE_ANGELSCRIPT
		typedef Tag<TagId::AngelscriptPlatform> AngelscriptPlatformTag;
#	endif
#	ifdef USE_BZIP2
		typedef Tag<TagId::Bzip2Platform>       Bzip2PlatformTag;
#	endif
#	ifdef USE_CARBON
		typedef Tag<TagId::CarbonPlatform>      CarbonPlatformTag;
#	endif
#	ifdef USE_CG
		typedef Tag<TagId::CgPlatform>          CgPlatformTag;
#	endif
#	ifdef USE_CURL
		typedef Tag<TagId::CurlPlatform>        CurlPlatformTag;
#	endif
#	ifdef USE_D3D9
		typedef Tag<TagId::D3d9Platform>        D3d9PlatformTag;
#	endif
#	ifdef USE_DINPUT9
		typedef Tag<TagId::Dinput9Platform>     Dinput9PlatformTag;
#	endif
#	ifdef USE_DSOUND9
		typedef Tag<TagId::Dsound9Platform>     Dsound9PlatformTag;
#	endif
#	ifdef USE_EXPAT
		typedef Tag<TagId::ExpatPlatform>       ExpatPlatformTag;
#	endif
#	ifdef USE_FFMPEG
		typedef Tag<TagId::FfmpegPlatform>      FfmpegPlatformTag;
#	endif
#	ifdef USE_FLAC
		typedef Tag<TagId::FlacPlatform>        FlacPlatformTag;
#	endif
#	ifdef USE_FREETYPE
		typedef Tag<TagId::FreetypePlatform>    FreetypePlatformTag;
#	endif
#	ifdef USE_GIF
		typedef Tag<TagId::GifPlatform>         GifPlatformTag;
#	endif
#	ifdef USE_JPEG
		typedef Tag<TagId::JpegPlatform>        JpegPlatformTag;
#	endif
#	ifdef USE_LUA
		typedef Tag<TagId::LuaPlatform>         LuaPlatformTag;
#	endif
#	ifdef USE_MINIZIP
		typedef Tag<TagId::MinizipPlatform>     MinizipPlatformTag;
#	endif
#	ifdef USE_MNG
		typedef Tag<TagId::MngPlatform>         MngPlatformTag;
#	endif
#	ifdef USE_OGG
		typedef Tag<TagId::OggPlatform>         OggPlatformTag;
#	endif
#	ifdef USE_OPENAL
		typedef Tag<TagId::OpenalPlatform>      OpenalPlatformTag;
#	endif
#	ifdef USE_OPENEXR
		typedef Tag<TagId::OpenexrPlatform>     OpenexrPlatformTag;
#	endif
#	ifdef USE_OPENGL
		typedef Tag<TagId::OpenglPlatform>      OpenglPlatformTag;
#	endif
#	ifdef USE_PNG
		typedef Tag<TagId::PngPlatform>         PngPlatformTag;
#	endif
#	ifdef USE_POSIX
		typedef Tag<TagId::PosixPlatform>       PosixPlatformTag;
#	endif
#	ifdef USE_PYTHON
		typedef Tag<TagId::PythonPlatform>      PythonPlatformTag;
#	endif
#	ifdef USE_SPHINX
		typedef Tag<TagId::SphinxPlatform>      SphinxPlatformTag;
#	endif
#	ifdef USE_SQUIRREL
		typedef Tag<TagId::SquirrelPlatform>    SquirrelPlatformTag;
#	endif
#	ifdef USE_THEORA
		typedef Tag<TagId::TheoraPlatform>      TheoraPlatformTag;
#	endif
#	ifdef USE_TIFF
		typedef Tag<TagId::TiffPlatform>        TiffPlatformTag;
#	endif
#	ifdef USE_UNGIF
		typedef Tag<TagId::UngifPlatform>       UngifPlatformTag;
#	endif
#	ifdef USE_VORBIS
		typedef Tag<TagId::VorbisPlatform>      VorbisPlatformTag;
#	endif
#	ifdef USE_WIN32
		typedef Tag<TagId::Win32Platform>       Win32PlatformTag;
#	endif
#	ifdef USE_X11
		typedef Tag<TagId::X11Platform>         X11PlatformTag;
#	endif
#	ifdef USE_ZIP
		typedef Tag<TagId::ZipPlatform>         ZipPlatformTag;
#	endif
#	ifdef USE_ZLIB
		typedef Tag<TagId::ZlibPlatform>        ZlibPlatformTag;
#	endif
	}
}

#endif
