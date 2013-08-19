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
