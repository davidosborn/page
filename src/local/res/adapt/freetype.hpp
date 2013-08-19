#ifndef    page_local_res_adapt_freetype_hpp
#   define page_local_res_adapt_freetype_hpp

#	include <memory> // unique_ptr

#	include <ft2build.h>
#	include FT_FREETYPE_H // FT_Open_Args

namespace page
{
	namespace res
	{
		class Pipe;
		class Stream;

		namespace freetype
		{
			FT_Library &GetLib();

			struct OpenArgs
			{
				explicit OpenArgs(const Pipe &);
				explicit OpenArgs(std::unique_ptr<Stream> &);
				~OpenArgs();

				const FT_Open_Args &operator *() const;
				const FT_Open_Args *operator ->() const;
				const FT_Open_Args *Get() const;

				protected:
				void Init();

				private:
				static unsigned long Read(FT_Stream, unsigned long, unsigned char *, unsigned long);
				static void Close(FT_Stream);

				std::unique_ptr<Stream> stream;
				FT_Open_Args args;
				unsigned off;
			};

			// numeric conversions
			float FracToFloat(FT_Pos);
			float FixedToFloat(FT_Fixed);
			FT_Pos FloatToFrac(float);
			FT_Fixed FloatToFixed(float);
		}

		using namespace freetype;
	}
}

#endif
