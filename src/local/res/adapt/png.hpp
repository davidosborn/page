#ifndef    page_local_res_adapt_png_hpp
#   define page_local_res_adapt_png_hpp

#	include <iosfwd> // ostream

#	include <png.h> // png_bytep, png_infop, png_size_t, png_structp

namespace page
{
	namespace res
	{
		class Stream;

		namespace png
		{
			// input
			struct ReadInfo
			{
				ReadInfo(Stream &);
				~ReadInfo();

				png_structp png;
				png_infop info;

				private:
				static void Read(png_structp, png_bytep, png_size_t);
			};

			// output
			struct WriteInfo
			{
				WriteInfo(std::ostream &);
				~WriteInfo();

				png_structp png;
				png_infop info;

				private:
				static void Write(png_structp, png_bytep, png_size_t);
				static void Flush(png_structp);
			};
		}

		using namespace png;
	}
}

#endif
