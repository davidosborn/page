#include <cmath> // fmod

#include "../cache/proxy/Proxy.hpp"
#include "../res/type/Sound.hpp" // GetDuration
#include "channel/AmbientChannel.hpp" // AmbientChannel::SetVolume
#include "Sound.hpp"

namespace page { namespace aud
{
	// construct
	Sound::Sound(const cache::Proxy<res::Sound> &sound, bool loop, bool fade, float fadeDuration) :
		sound(sound.Copy()), playing(true), paused(false), loop(loop),
		fade(fade), fadeDuration(fadeDuration), level(!fade),
		playPosition(0), volume(0), channel(0) {}

	// attributes
	const cache::Proxy<res::Sound> &Sound::GetSound() const
	{
		return *sound;
	}

	// state
	bool Sound::IsAlive() const
	{
		return playing || level;
	}
	bool Sound::IsPlaying() const
	{
		return IsAlive() && !paused;
	}
	bool Sound::IsStopping() const
	{
		return !playing;
	}
	bool Sound::IsLooping() const
	{
		return loop;
	}
	float Sound::GetFadeDuration() const
	{
		return fade ? fadeDuration : 0;
	}
	float Sound::GetLevel() const
	{
		return level;
	}
	float Sound::GetPlayPosition() const
	{
		return playPosition;
	}
	float Sound::GetVolume() const
	{
		return volume;
	}

	// update
	void Sound::Update(float deltaTime)
	{
		if (IsAlive() && !paused)
		{
			// update play position
			playPosition += deltaTime;
			float duration = GetDuration(**sound);
			if (playPosition >= duration)
			{
				if (!loop)
				{
					playing = false;
					playPosition = 0;
				}
				else playPosition = std::fmod(playPosition, duration);
			}
			// update level
			level = playing ?
				std::min(level + deltaTime / fadeDuration, 1.f) :
				std::max(level - deltaTime / fadeDuration, 0.f);
		}
	}

	// modifiers
	void Sound::Stop()
	{
		Stop(fade);
	}
	void Sound::Stop(bool fade)
	{
		Stop(fade, fadeDuration);
	}
	void Sound::Stop(bool fade, float fadeDuration)
	{
		playing = false;
		if (!fade) level = 0;
		if (channel) channel->Stop(fade ? fadeDuration : 0);
	}
	void Sound::Pause()
	{
		paused = true;
	}
	void Sound::Resume()
	{
		if (paused) paused = false;
		else if (!playing)
		{
			playing = true;
			level = !fade;
			playPosition = 0;
			channel = 0;
		}
	}
	void Sound::SetVolume(float volume)
	{
		this->volume = volume;
		if (channel) channel->SetVolume(volume);
	}

	// channel binding
	bool Sound::HasChannel() const
	{
		return channel;
	}
	AmbientChannel &Sound::GetChannel() const
	{
		assert(channel);
		return *channel;
	}
	void Sound::Bind(AmbientChannel &channel)
	{
		this->channel = &channel;
	}
	void Sound::ReleaseChannel()
	{
		if (channel)
		{
			channel->Stop(0);
			channel = 0;
		}
	}
}}
