#ifndef    page_local_res_adapt_zip_hpp
#   define page_local_res_adapt_zip_hpp

#	include <cerrno>

#	include <zip.h> // zip, zip_file

namespace page
{
	namespace res
	{
		void ZipError(int ze, int se = errno);
		void ZipError(zip *);
		void ZipError(zip_file *);
	}
}

#endif
