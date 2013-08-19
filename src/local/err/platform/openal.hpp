#ifndef    page_local_err_platform_openal_hpp
#   define page_local_err_platform_openal_hpp

#	include <string>

#	include <AL/al.h> // ALenum

namespace page { namespace err { namespace openal
{
	void ResetError();
	void CheckError();
	std::string GetErrorString(ALenum error);
}}}

#endif
