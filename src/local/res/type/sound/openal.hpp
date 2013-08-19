#ifndef    page_local_res_type_sound_openal_hpp
#   define page_local_res_type_sound_openal_hpp

#	include <AL/al.h> // ALenum

namespace page
{
	namespace res
	{
		class Sound;

		namespace openal
		{
			ALenum GetFormat(const Sound &);
		}
	}
}

#endif
