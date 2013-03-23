/**
 * @section license
 *
 * Copyright (c) 2006-2013 David Osborn
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

#include <cmath> // floor
#include <cstdlib> // ldiv
#include <utility> // move

#include "../../err/Exception.hpp"
#include "../pipe/Pipe.hpp"
#include "../pipe/Stream.hpp"
#include "freetype.hpp"

namespace page
{
	namespace res
	{
		namespace freetype
		{
			FT_Library &GetLib()
			{
				struct Initer
				{
					Initer()
					{
						if (FT_Init_FreeType(&lib))
							THROW((err::Exception<err::ResModuleTag, err::FreetypePlatformTag>("failed to initialize") <<
								boost::errinfo_api_function("FT_Init_FreeType")))
					}
					~Initer()
					{
						FT_Done_FreeType(lib);
					}

					FT_Library lib;
				};
				static Initer initer;
				return initer.lib;
			}

			OpenArgs::OpenArgs(const Pipe &pipe) :
				stream(pipe.Open()), off(0)
			{
				Init();
			}
			OpenArgs::OpenArgs(std::unique_ptr<Stream> &stream) :
				stream(std::move(stream)), off(stream->Tell())
			{
				Init();
			}
			OpenArgs::~OpenArgs()
			{
				delete args.stream;
			}

			const FT_Open_Args &OpenArgs::operator *() const { return args; }
			const FT_Open_Args *OpenArgs::operator ->() const { return &args; }
			const FT_Open_Args *OpenArgs::Get() const { return &args; }

			void OpenArgs::Init()
			{
				args.flags = FT_OPEN_STREAM;
				args.stream = new FT_StreamRec;
				args.stream->base = 0;
				args.stream->size = stream->Size();
				args.stream->pos = stream->Tell();
				args.stream->descriptor.pointer = this;
				// FIXME: if pipe knows its path, set args.stream->pathname
				args.stream->read = Read;
				args.stream->close = Close;
			}

			unsigned long OpenArgs::Read(FT_Stream stream, unsigned long pos, unsigned char *s, unsigned long n)
			{
				const OpenArgs &args(*static_cast<OpenArgs *>(stream->descriptor.pointer));
				try
				{
					args.stream->Seek(args.off + pos);
				}
				catch (const Exception<err::EndOfStreamTag>::Permutation &)
				{
					return 0;
				}
				return n ? args.stream->ReadSome(s, n) : 0;
			}
			void OpenArgs::Close(FT_Stream stream)
			{
				OpenArgs &args(*static_cast<OpenArgs *>(stream->descriptor.pointer));
				args.stream.reset();
			}

			// numeric conversions
			float FracToFloat(FT_Pos frac)
			{
				std::ldiv_t d(std::ldiv(frac, 64));
				return d.quot + d.rem / 64.f;
			}
			float FixedToFloat(FT_Fixed fixed)
			{
				std::ldiv_t d(std::ldiv(fixed, 65536));
				return d.quot + d.rem / 65536.f;
			}
			FT_Pos FloatToFrac(float f)
			{
				float i = std::floor(f);
				return (FT_Pos(i) << 6) + FT_Pos((f - i) * 64.f);
			}
			FT_Fixed FloatToFixed(float f)
			{
				float i = std::floor(f);
				return (FT_Pos(i) << 16) + FT_Pos((f - i) * 65536.f);
			}
		}
	}
}
