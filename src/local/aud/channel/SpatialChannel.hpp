#ifndef    page_local_aud_channel_SpatialChannel_hpp
#   define page_local_aud_channel_SpatialChannel_hpp

#	include "../../util/Identifiable.hpp" // Identifiable::Id
#	include "../Channel.hpp"

namespace page { namespace phys { class Sound; }}

namespace page { namespace aud
{
	struct SpatialChannel : virtual Channel
	{
		// construct
		explicit SpatialChannel(const phys::Sound &);

		// state
		bool IsAudible() const;

		// update
		void DoUpdate(float deltaTime);

		// modifiers
		void Occlude();

		// sound access
		phys::Sound *GetSound() const;
		util::Identifiable::Id GetId() const;

		private:
		// state
		float DoGetLinearVolume() const;

		util::Identifiable::Id id;
		bool occluded;
		float level;
	};
}}

#endif
