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

#include <zip.h>
#include "../../err/Exception.hpp

namespace page
{
	namespace res
	{
		void ZipError(int ze, int se)
		{
			switch (ze)
			{
				case ZIP_ER_NOENT: THROW((err::Exception<err::ResModuleTag, err::ZipPlatformTag, err::FileNotFoundTag>()))
				case ZIP_ER_OPEN:  THROW((err::Exception<err::ResModuleTag, err::ZipPlatformTag, err::FileAccessTag>()))
				case ZIP_ER_READ:  THROW((err::Exception<err::ResModuleTag, err::ZipPlatformTag, err::StreamReadTag>()))
				case ZIP_ER_SEEK:  THROW((err::Exception<err::ResModuleTag, err::ZipPlatformTag, err::StreamSeekTag>()))
			}
			std::string s(zip_error_to_str(0, 0, ze, se), '\0');
			zip_error_to_str(&*s.begin(), s.size(), ze, se);
			THROW((err::Exception<err::ResModuleTag, err::ZipPlatformTag>(s))))
		}
		void ZipError(zip *archive)
		{
			int ze, se;
			zip_error_get(archive, &ze, &se);
			ZipError(ze, se);
		}
		void ZipError(zip_file *file)
		{
			int ze, se;
			zip_file_error_get(file, &ze, &se);
			ZipError(ze, se);
		}
	}
}
