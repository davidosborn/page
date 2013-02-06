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

#include <cassert>
#include <memory> // unique_ptr

#include "../../err/Exception.hpp"
#include "../pipe/Pipe.hpp" // Pipe::Open
#include "../Stream.hpp"
#include "flac.hpp" // File, WriteCallback

namespace page
{
	namespace res
	{
		namespace flac
		{
			namespace
			{
				// stream information
				struct ClientData
				{
					explicit ClientData(const Pipe &pipe, const WriteCallback &write) :
						stream(pipe.Open()), write(write), written(false) {}

					const std::unique_ptr<Stream> stream;
					WriteCallback write;
					bool written; // has the first frame been decoded?
				};

				// stream callbacks
				FLAC__StreamDecoderReadStatus Read(const FLAC__StreamDecoder *sd, FLAC__byte s[], size_t *n, void *param)
				{
					const ClientData *cd = reinterpret_cast<ClientData *>(param);
					if (*n)
					{
						try
						{
							if (!(*n = cd->stream->ReadSome(s, *n)))
								return FLAC__STREAM_DECODER_READ_STATUS_END_OF_STREAM;
						}
						catch (...)
						{
							return FLAC__STREAM_DECODER_READ_STATUS_ABORT;
						}
					}
					return FLAC__STREAM_DECODER_READ_STATUS_CONTINUE;
				}
				FLAC__StreamDecoderSeekStatus Seek(const FLAC__StreamDecoder *sd, FLAC__uint64 pos, void *param)
				{
					const ClientData *cd = reinterpret_cast<ClientData *>(param);
					try
					{
						cd->stream->Seek(pos);
					}
					catch (...)
					{
						return FLAC__STREAM_DECODER_SEEK_STATUS_ERROR;
					}
					return FLAC__STREAM_DECODER_SEEK_STATUS_OK;
				}
				FLAC__StreamDecoderTellStatus Tell(const FLAC__StreamDecoder *sd, FLAC__uint64 *pos, void *param)
				{
					const ClientData *cd = reinterpret_cast<ClientData *>(param);
					try
					{
						*pos = cd->stream->Tell();
					}
					catch (...)
					{
						return FLAC__STREAM_DECODER_TELL_STATUS_ERROR;
					}
					return FLAC__STREAM_DECODER_TELL_STATUS_OK;
				}
				FLAC__StreamDecoderLengthStatus Length(const FLAC__StreamDecoder *sd, FLAC__uint64 *size, void *param)
				{
					const ClientData *cd = reinterpret_cast<ClientData *>(param);
					try
					{
						*size = cd->stream->Size();
					}
					catch (...)
					{
						return FLAC__STREAM_DECODER_LENGTH_STATUS_ERROR;
					}
					return FLAC__STREAM_DECODER_LENGTH_STATUS_OK;
				}
				FLAC__bool Eof(const FLAC__StreamDecoder *sd, void *param)
				{
					const ClientData *cd = reinterpret_cast<ClientData *>(param);
					return cd->stream->Eof();
				}
				FLAC__StreamDecoderWriteStatus Write(const FLAC__StreamDecoder *sd, const FLAC__Frame *frame, const FLAC__int32 *const buffers[], void *param)
				{
					ClientData *cd = reinterpret_cast<ClientData *>(param);
					if (cd->write)
					{
						try
						{
							cd->write(sd, frame, buffers);
						}
						catch (...)
						{
							return FLAC__STREAM_DECODER_WRITE_STATUS_ABORT;
						}
					}
					cd->written = true;
					return FLAC__STREAM_DECODER_WRITE_STATUS_CONTINUE;
				}
				void Error(const FLAC__StreamDecoder *, FLAC__StreamDecoderErrorStatus e, void *)
				{
					// NOTE: disabled; use return values rather than exceptions
/*					switch (e)
					{
						case FLAC__STREAM_DECODER_ERROR_STATUS_LOST_SYNC:          THROW((err::Exception<err::ResModuleTag, err::FlacPlatformTag, err::FormatTag>("lost synchronization")))
						case FLAC__STREAM_DECODER_ERROR_STATUS_BAD_HEADER:         THROW((err::Exception<err::ResModuleTag, err::FlacPlatformTag, err::FormatTag>("corrupt header")))
						case FLAC__STREAM_DECODER_ERROR_STATUS_FRAME_CRC_MISMATCH: THROW((err::Exception<err::ResModuleTag, err::FlacPlatformTag, err::FormatTag>("CRC mismatch")))
						case FLAC__STREAM_DECODER_ERROR_STATUS_UNPARSEABLE_STREAM: THROW((err::Exception<err::ResModuleTag, err::FlacPlatformTag, err::FormatTag>("unrecognized fields")))
						default:                                                   THROW((err::Exception<err::ResModuleTag, err::FlacPlatformTag, err::FormatTag>()))
					}*/
				}

				// smart pointer deleter
				struct Deleter
				{
					void operator ()(FLAC__StreamDecoder *sd, const ClientData *cd) const
					{
						FLAC__stream_decoder_finish(sd);
						delete cd;
						FLAC__stream_decoder_delete(sd);
					}
				};

				// initialization helpers
				void CheckInitStatus(FLAC__StreamDecoderInitStatus status)
				{
					switch (status)
					{
						case FLAC__STREAM_DECODER_INIT_STATUS_OK: break;
						case FLAC__STREAM_DECODER_INIT_STATUS_UNSUPPORTED_CONTAINER:   THROW((err::Exception<err::ResModuleTag, err::FlacPlatformTag>("unsupported container")))
						case FLAC__STREAM_DECODER_INIT_STATUS_INVALID_CALLBACKS:       THROW((err::Exception<err::ResModuleTag, err::FlacPlatformTag>("invalid callbacks")))
						case FLAC__STREAM_DECODER_INIT_STATUS_MEMORY_ALLOCATION_ERROR: THROW((err::Exception<err::ResModuleTag, err::FlacPlatformTag, err::AllocTag>()))
						case FLAC__STREAM_DECODER_INIT_STATUS_ERROR_OPENING_FILE:      THROW((err::Exception<err::ResModuleTag, err::FlacPlatformTag, err::FileAccessTag>()))
						case FLAC__STREAM_DECODER_INIT_STATUS_ALREADY_INITIALIZED:     THROW((err::Exception<err::ResModuleTag, err::FlacPlatformTag>("already initialized")))
						default:                                                       THROW((err::Exception<err::ResModuleTag, err::FlacPlatformTag>()))
					}
				}
				bool Init(FLAC__StreamDecoder *sd, const ClientData *cd, FLAC__StreamDecoderInitStatus status)
				{
					if (status == FLAC__STREAM_DECODER_INIT_STATUS_UNSUPPORTED_CONTAINER)
						return false;
					CheckInitStatus(status);
					// read STREAMINFO block and decode first frame
					do
					{
						if (!FLAC__stream_decoder_process_single(sd))
						{
							FLAC__stream_decoder_finish(sd);
							return false;
						}
					}
					while (!cd->written);
					return true;
				}
				bool InitFlac(const std::unique_ptr<FLAC__StreamDecoder> &sd, const Pipe &pipe, const WriteCallback &write)
				{
					const std::unique_ptr<ClientData> cd(new ClientData(pipe, write));
					if (Init(sd.get(), cd.get(), FLAC__stream_decoder_init_stream(
						sd.get(), Read, Seek, Tell, Length, Eof, Write, 0, Error, cd.get())))
					{
						sd.reset(sd.release(),
							std::bind(Deleter(), std::placeholders::_1, cd.release()));
						return true;
					}
					return false;
				}
				bool InitOggFlac(const std::unique_ptr<FLAC__StreamDecoder> &sd, const Pipe &pipe, const WriteCallback &write)
				{
					const std::unique_ptr<ClientData> cd(new ClientData(pipe, write));
					if (Init(sd.get(), cd.get(), FLAC__stream_decoder_init_ogg_stream(
						sd.get(), Read, Seek, Tell, Length, Eof, Write, 0, Error, cd.get())))
					{
						sd.reset(sd.release(),
							std::bind(Deleter(), std::placeholders::_1, cd.release()));
						return true;
					}
					return false;
				}
			}

			// error handling
			void CheckError(FLAC__StreamDecoderState e)
			{
				switch (e)
				{
					case FLAC__STREAM_DECODER_OGG_ERROR:               THROW((err::Exception<err::ResModuleTag, err::OggPlatformTag>()))
					case FLAC__STREAM_DECODER_SEEK_ERROR:              THROW((err::Exception<err::ResModuleTag, err::FlacPlatformTag, err::StreamSeekTag>()))
					case FLAC__STREAM_DECODER_ABORTED:                 THROW((err::Exception<err::ResModuleTag, err::FlacPlatformTag, err::StreamReadTag>()))
					case FLAC__STREAM_DECODER_MEMORY_ALLOCATION_ERROR: THROW((err::Exception<err::ResModuleTag, err::FlacPlatformTag, err::AllocTag>()))
					case FLAC__STREAM_DECODER_UNINITIALIZED:           THROW((err::Exception<err::ResModuleTag, err::FlacPlatformTag>("not initialized")))
				}
			}
			void CheckError(const FLAC__StreamDecoder *sd)
			{
				CheckError(FLAC__stream_decoder_get_state(sd));
			}

			// streaming
			bool Check(const Pipe &pipe)
			{
				// FIXME: test for FLAC format by checking for the FLAC
				// signature or, if the Ogg signature is found, by parsing the
				// Ogg header and checking for the FLAC signature afterwards
				try
				{
					return Open(pipe, nullptr) != nullptr;
				}
				catch (...)
				{
					return false;
				}
			}
			File Open(const Pipe &pipe, const WriteCallback &write)
			{
				const std::unique_ptr<FLAC__StreamDecoder> sd(
					FLAC__stream_decoder_new(), FLAC__stream_decoder_delete);
				return
					InitFlac   (sd, pipe, write) ||
					InitOggFlac(sd, pipe, write) ?
					File(sd.release(), sd.get_deleter()) :
					File();
			}
		}
	}
}
