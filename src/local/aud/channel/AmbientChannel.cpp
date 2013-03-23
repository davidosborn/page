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
 *
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

#include <algorithm> // max, min

#include "../../math/float.hpp" // DecibelToLinear
#include "../../math/interp.hpp" // HermiteScale
#include "../Sound.hpp" // Sound::{Get{FadeDuration,Level,Volume},IsStopping}
#include "AmbientChannel.hpp"

namespace page { namespace aud
{
	// construct
	AmbientChannel::AmbientChannel(const Sound &sound) :
		Channel(sound.GetPlayPosition()), playing(!sound.IsStopping()),
		fadeDuration(sound.GetFadeDuration()), level(sound.GetLevel()),
		volume(sound.GetVolume()) {}

	// state
	bool AmbientChannel::IsAlive() const
	{
		return playing || level;
	}

	// update
	void AmbientChannel::DoUpdate(float deltaTime)
	{
		level = playing ?
			std::min(level + deltaTime / fadeDuration, 1.f) :
			std::max(level - deltaTime / fadeDuration, 0.f);
	}

	// modifiers
	void AmbientChannel::Stop()
	{
		playing = false;
		if (!fadeDuration) level = 0;
	}
	void AmbientChannel::Stop(float fadeDuration)
	{
		playing = false;
		if (!fadeDuration) level = 0;
		else this->fadeDuration = fadeDuration;
	}
	void AmbientChannel::SetVolume(float volume)
	{
		this->volume = volume;
	}

	// state
	float AmbientChannel::DoGetLinearVolume() const
	{
		return math::DecibelToLinear(volume) *
			math::HermiteScale(level, fadeExponent);
	}
}}
