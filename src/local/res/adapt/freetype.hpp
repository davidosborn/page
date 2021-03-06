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
