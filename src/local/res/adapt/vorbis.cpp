#include <memory> // unique_ptr

#include "../../err/Exception.hpp"
#include "../pipe/Pipe.hpp"
#include "../pipe/Stream.hpp"
#include "vorbis.hpp" // File

namespace page
{
	namespace res
	{
		namespace vorbis
		{
			namespace
			{
				/*-----------------+
				| stream callbacks |
				+-----------------*/

				std::size_t Read(void *s, std::size_t size, std::size_t n, void *datasource)
				{
					Stream *stream = reinterpret_cast<Stream *>(datasource);
					return stream->ReadSome(s, size * n);
				}

				int Seek(void *datasource, ogg_int64_t offset, int whence)
				{
					Stream *stream = reinterpret_cast<Stream *>(datasource);
					Stream::SeekOrigin origin;
					switch (whence)
					{
						case SEEK_SET: origin = Stream::begSeekOrigin; break;
						case SEEK_CUR: origin = Stream::curSeekOrigin; break;
						case SEEK_END: origin = Stream::endSeekOrigin; break;
						default: return -1;
					}
					stream->Seek(offset, origin);
					return 0;
				}

				int Close(void *datasource)
				{
					Stream *stream = reinterpret_cast<Stream *>(datasource);
					delete stream;
					return 0;
				}

				long Tell(void *datasource)
				{
					Stream *stream = reinterpret_cast<Stream *>(datasource);
					return stream->Tell();
				}

				/*----------------------+
				| smart-pointer deleter |
				+----------------------*/

				struct Deleter
				{
					void operator ()(OggVorbis_File *vf) const
					{
						ov_clear(vf);
						delete vf;
					}
				};
			}

			/*---------------+
			| error handling |
			+---------------*/

			void CheckError(int e)
			{
				if (e >= 0) return;
				switch (e)
				{
					case OV_FALSE:      THROW((err::Exception<err::ResModuleTag, err::VorbisPlatformTag>("no data")))
					case OV_HOLE:       THROW((err::Exception<err::ResModuleTag, err::VorbisPlatformTag, err::FormatTag>("corrupt bitstream")))
					case OV_EREAD:      THROW((err::Exception<err::ResModuleTag, err::VorbisPlatformTag, err::StreamReadTag>()))
					case OV_EFAULT:     THROW((err::Exception<err::ResModuleTag, err::VorbisPlatformTag>("internal error")))
					case OV_EIMPL:      THROW((err::Exception<err::ResModuleTag, err::VorbisPlatformTag>("feature not implemented")))
					case OV_EINVAL:     THROW((err::Exception<err::ResModuleTag, err::VorbisPlatformTag, err::ParameterTag>("invalid argument")))
					case OV_ENOTVORBIS: THROW((err::Exception<err::ResModuleTag, err::VorbisPlatformTag, err::FormatTag>("unrecognized format")))
					case OV_EBADHEADER: THROW((err::Exception<err::ResModuleTag, err::VorbisPlatformTag, err::FormatTag>("corrupt header")))
					case OV_EVERSION:   THROW((err::Exception<err::ResModuleTag, err::VorbisPlatformTag, err::FormatTag>("version mismatch")))
					case OV_EBADLINK:   THROW((err::Exception<err::ResModuleTag, err::VorbisPlatformTag, err::FormatTag>("corrupt link")))
					case OV_ENOSEEK:    THROW((err::Exception<err::ResModuleTag, err::VorbisPlatformTag, err::StreamSeekTag>("not seekable")))
					default:            THROW((err::Exception<err::ResModuleTag, err::VorbisPlatformTag>()))
				}
			}

			/*----------+
			| streaming |
			+----------*/

			bool Check(const Pipe &pipe)
			{
				const std::unique_ptr<Stream> stream(pipe.Open());
				OggVorbis_File vf;
				ov_callbacks cb = {Read, Seek, Close, Tell};
				int result = ov_test_callbacks(stream.get(), &vf, 0, 0, cb);
				if (!result)
				{
					stream.release();
					ov_clear(&vf);
				}
				return !result;
			}

			File Open(const Pipe &pipe)
			{
				auto vf(new OggVorbis_File);

				try
				{
					const std::unique_ptr<Stream> stream(pipe.Open());
					ov_callbacks cb = {Read, Seek, Close, Tell};
					if (ov_test_callbacks(stream.get(), vf, 0, 0, cb))
						return nullptr;
					stream.release();

					try
					{
						CheckError(ov_test_open(vf.get()));
						return File(vf2, Deleter());
					}
					catch (...)
					{
						ov_clear(vf.get());
						throw;
					}
				}
				catch (...)
				{
					delete vf;
					throw;
				}
			}
		}
	}
}
