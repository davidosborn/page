/**
 * @copyright
 *
 * Copyright (c) 2006-2014 David Osborn
 *
 * Permission is granted to use and redistribute this software in source and
 * binary form, with or without modification, subject to the following
 * conditions:
 *
 * 1. Redistributions in source form must retain the above copyright notice,
 *    this list of conditions, and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions, and the following disclaimer in the same place
 *    and form as other copyright, license, and disclaimer information.
 *
 * 3. Redistributions in binary form must also include an acknowledgement in the
 *    same place and form as other acknowledgements (such as the credits),
 *    similar in substance to the following:
 *
 *       Portions of this software are based on the work of David Osborn.
 *
 * This software is provided "as is", without any express or implied warranty.
 * In no event will the authors be liable for any damages arising out of the use
 * of this software.
 */

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
		return sound;
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
			float duration = GetDuration(*sound);
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
