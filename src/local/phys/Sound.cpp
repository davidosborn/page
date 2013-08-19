#include <cmath> // fmod

#include "../cache/proxy/Proxy.hpp"
#include "../res/type/Sound.hpp" // GetDuration
#include "Sound.hpp"

namespace page
{
	namespace phys
	{
		// construct
		Sound::Sound(const cache::Proxy<res::Sound> &sound, bool loop) :
			sound(sound.Copy()), playing(true), loop(loop), playPosition(0) {}

		// state
		bool Sound::IsPlaying() const
		{
			return playing;
		}
		bool Sound::IsLooping() const
		{
			return loop;
		}
		float Sound::GetPlayPosition() const
		{
			return playPosition;
		}

		// sound access
		const cache::Proxy<res::Sound> &Sound::GetSound() const
		{
			return *sound;
		}

		// update
		void Sound::Update(float deltaTime)
		{
			if (playing)
			{
				playPosition += deltaTime;
				float duration = GetDuration(**sound);
				if (playPosition >= duration)
				{
					if (!loop)
					{
						playing = false;
						playPosition = duration;
					}
					else playPosition = std::fmod(playPosition, duration);
				}
			}
		}

		// frame serialization
		Frame Sound::GetFrame() const
		{
			return
				Position::GetFrame() +
				Volume::GetFrame();
		}
		void Sound::Update(const Frame &frame)
		{
			Position::Update(frame);
			Volume::Update(frame);
		}
	}
}
