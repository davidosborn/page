#include <cmath> // fmod

#include "../../res/type/Sound.hpp" // GetDuration
#include "Sound.hpp"

namespace page { namespace phys
{
	/*-------------+
	| constructors |
	+-------------*/

	Sound::Sound(const cache::Proxy<res::Sound> &sound, bool loop) :
		sound(sound), loop(loop) {}

	/*----------+
	| accessors |
	+----------*/

	const cache::Proxy<res::Sound> &Sound::GetSound() const
	{
		return *sound;
	}

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

	/*-------+
	| update |
	+-------*/

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

	/*--------------------+
	| Node implementation |
	+--------------------*/

	NodeType Sound::GetType() const override
	{
		return NodeType::sound;
	}
}}
