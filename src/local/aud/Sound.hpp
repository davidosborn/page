#ifndef    page_local_aud_Sound_hpp
#   define page_local_aud_Sound_hpp

#	include "../cache/proxy/Proxy.hpp"

namespace page { namespace res { class Sound; }}

namespace page { namespace aud
{
	class AmbientChannel;

	struct Sound
	{
		// construct
		explicit Sound(const cache::Proxy<res::Sound> &, bool loop,
			bool fade, float fadeDuration);

		// attributes
		const cache::Proxy<res::Sound> &GetSound() const;

		// state
		bool IsAlive() const;
		bool IsPlaying() const;
		bool IsStopping() const;
		bool IsLooping() const;
		float GetFadeDuration() const;
		float GetLevel() const;
		float GetPlayPosition() const;
		float GetVolume() const;

		// update
		void Update(float deltaTime);

		// modifiers
		void Stop();
		void Stop(bool fade);
		void Stop(bool fade, float fadeDuration);
		void Pause();
		void Resume();
		void SetVolume(float);

		// channel binding
		// state changes will propogate to bound channel
		bool HasChannel() const;
		AmbientChannel &GetChannel() const;
		void Bind(AmbientChannel &);
		void ReleaseChannel();

		private:
		cache::Proxy<res::Sound> sound;
		bool playing, paused, loop, fade;
		float fadeDuration, level, playPosition, volume;
		AmbientChannel *channel;
	};
}}

#endif
