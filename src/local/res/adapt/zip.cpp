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
