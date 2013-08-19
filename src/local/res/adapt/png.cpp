#include <iostream> // clog

#include "../../err/Exception.hpp"
#include "../../log/manip.hpp" // Warning
#include "../pipe/Stream.hpp"
#include "png.hpp"

namespace page
{
	namespace res
	{
		namespace png
		{
			namespace
			{
				void Error(png_structp png, png_const_charp s)
				{
					THROW((err::Exception<err::ResModuleTag, err::PngPlatformTag>(s)))
				}
				void Warning(png_structp png, png_const_charp s)
				{
					std::clog << log::Warning << "png error: " << s << std::endl;
				}
			}

			// input
			ReadInfo::ReadInfo(Stream &stream)
			{
				if (!(png = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, Error, Warning)))
					THROW((err::Exception<err::ResModuleTag, err::PngPlatformTag>("failed to initialize") <<
						boost::errinfo_api_function("png_create_read_struct")))
				if (!(info = png_create_info_struct(png)))
				{
					png_destroy_read_struct(&png, 0, 0);
					THROW((err::Exception<err::ResModuleTag, err::PngPlatformTag>("failed to initialize") <<
						boost::errinfo_api_function("png_create_info_struct")))
				}
				png_set_read_fn(png, &stream, &ReadInfo::Read);
			}
			ReadInfo::~ReadInfo()
			{
				png_destroy_read_struct(&png, &info, 0);
			}
			void ReadInfo::Read(png_structp png, png_bytep s, png_size_t n)
			{
				Stream *stream = static_cast<Stream *>(png_get_io_ptr(png));
				stream->Read(s, n);
			}

			// output
			WriteInfo::WriteInfo(std::ostream &os)
			{
				if (!(png = png_create_write_struct(PNG_LIBPNG_VER_STRING, 0, Error, Warning)))
					THROW((err::Exception<err::ResModuleTag, err::PngPlatformTag>("failed to initialize") <<
						boost::errinfo_api_function("png_create_write_struct")))
				if (!(info = png_create_info_struct(png)))
				{
					png_destroy_write_struct(&png, 0);
					THROW((err::Exception<err::ResModuleTag, err::PngPlatformTag>("failed to initialize") <<
						boost::errinfo_api_function("png_create_info_struct")))
				}
				png_set_write_fn(png, &os, &WriteInfo::Write, &WriteInfo::Flush);
			}
			WriteInfo::~WriteInfo()
			{
				png_destroy_write_struct(&png, &info);
			}
			void WriteInfo::Write(png_structp png, png_bytep s, png_size_t n)
			{
				std::ostream *os = reinterpret_cast<std::ostream *>(png_get_io_ptr(png));
				if (!os->write(reinterpret_cast<const char *>(s), n))
					THROW((err::Exception<err::ResModuleTag, err::PngPlatformTag, err::StreamWriteTag>() <<
						boost::errinfo_api_function("std::ostream::write")))
			}
			void WriteInfo::Flush(png_structp png)
			{
				std::ostream *os = reinterpret_cast<std::ostream *>(png_get_io_ptr(png));
				if (!os->flush())
					THROW((err::Exception<err::ResModuleTag, err::PngPlatformTag, err::StreamWriteTag>("stream flush error") <<
						boost::errinfo_api_function("std::ostream::flush")))
			}
		}
	}
}
