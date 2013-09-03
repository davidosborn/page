#ifndef    page_local_aud_openal_Buffer_hpp
#   define page_local_aud_openal_Buffer_hpp

#	include <AL/al.h> // ALuint

#	include "../../cache/proxy/fwd.hpp" // Proxy
#	include "../../util/class/special_member_functions.hpp" // Uncopyable

namespace page { namespace res { class Sound; }}

namespace page { namespace aud { namespace openal
{
	struct Buffer : util::Uncopyable<Buffer>
	{
		// destroy
		virtual ~Buffer();

		// update
		virtual void Update() = 0;
	};

	// factory function
	Buffer *MakeBuffer(ALuint source, const cache::Proxy<res::Sound> &,
		bool loop, float playPosition);
}}}

#endif
