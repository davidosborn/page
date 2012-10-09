/**
 * @section license
 *
 * Copyright (c) 2006-2012 David Osborn
 *
 * Permission is granted to use and redistribute this software in source and
 * binary form, with or without modification, subject to the following
 * conditions:
 *
 * 1. Redistributions in source form must retain the above copyright notice,
 *    this list of conditions, and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions, and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution, and in the same
 *    place and form as other copyright, license, and disclaimer information.
 *
 * As a special exception, distributions of derivative works in binary form may
 * include an acknowledgement in place of the above copyright notice, this list
 * of conditions, and the following disclaimer in the documentation and/or other
 * materials provided with the distribution, and in the same place and form as
 * other acknowledgements, similar in substance to the following:
 *
 *    Portions of this software are based on the work of David Osborn.
 *
 * This software is provided "as is", without any express or implied warranty.
 * In no event will the authors be liable for any damages arising out of the use
 * of this software.
 */

#include <cmath> // fmod
#include "../cache/Proxy.hpp"
#include "../res/type/Sound.hpp" // GetDuration
#include "channel/AmbientChannel.hpp" // AmbientChannel::SetVolume
#include "Sound.hpp"

namespace page
{
	namespace aud
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
	}
}
