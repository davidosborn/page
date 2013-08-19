#ifndef    page_local_aud_openal_channel_AmbientChannel_hpp
#   define page_local_aud_openal_channel_AmbientChannel_hpp

#	include "../../channel/AmbientChannel.hpp"
#	include "../Channel.hpp"

namespace page { namespace aud { namespace openal
{
	struct AmbientChannel : Channel, aud::AmbientChannel
	{
		// construct
		AmbientChannel(const Sound &);

		private:
		// update
		void DoUpdate3();
	};
}}}

#endif
