#ifndef    page_local_aud_channel_AmbientChannel_hpp
#   define page_local_aud_channel_AmbientChannel_hpp

#	include "../Channel.hpp"

namespace page { namespace aud
{
	class Sound;

	struct AmbientChannel : virtual Channel
	{
		// construct
		explicit AmbientChannel(const Sound &);

		// state
		bool IsAlive() const;

		// update
		void DoUpdate(float deltaTime);

		// modifiers
		void Stop();
		void Stop(float fadeDuration);
		void SetVolume(float);

		private:
		// state
		float DoGetLinearVolume() const;

		bool playing;
		float fadeDuration, level, volume;
	};
}}

#endif
