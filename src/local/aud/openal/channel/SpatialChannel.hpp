#ifndef    page_local_aud_openal_channel_SpatialChannel_hpp
#   define page_local_aud_openal_channel_SpatialChannel_hpp

#	include "../../channel/SpatialChannel.hpp"
#	include "../Channel.hpp"

namespace page { namespace aud { namespace openal
{
	struct SpatialChannel : Channel, aud::SpatialChannel
	{
		// construct/destroy
		SpatialChannel(const phys::Sound &);
		~SpatialChannel();

		private:
		// update
		void DoUpdate3();
	};
}}}

#endif
