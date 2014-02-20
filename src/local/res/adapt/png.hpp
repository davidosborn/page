/**
 * @copyright
 *
 * Copyright (c) 2006-2014 David Osborn
 *
 * Permission is granted to use and redistribute this software in source and
 * binary form, with or without modification, subject to the following
 * conditions:
 *
 * 1. Redistributions in source form must retain the above copyright notice,
 *    this list of conditions, and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions, and the following disclaimer in the same place
 *    and form as other copyright, license, and disclaimer information.
 *
 * 3. Redistributions in binary form must also include an acknowledgement in the
 *    same place and form as other acknowledgements (such as the credits),
 *    similar in substance to the following:
 *
 *       Portions of this software are based on the work of David Osborn.
 *
 * This software is provided "as is", without any express or implied warranty.
 * In no event will the authors be liable for any damages arising out of the use
 * of this software.
 */

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
