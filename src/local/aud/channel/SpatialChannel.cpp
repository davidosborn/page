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

#include <algorithm> // max

#include "../../math/float.hpp" // DecibelToLinear, Inf
#include "../../math/interp.hpp" // HermiteScale
#include "../../phys/node/Sound.hpp" // Sound->util::Identifiable, Sound::Get{PlayPosition,Volume}
#include "SpatialChannel.hpp"

namespace page { namespace aud
{
	float fadeOutDuration = 1;

	// construct
	SpatialChannel::SpatialChannel(const phys::Sound &sound) :
		Channel(sound.GetPlayPosition()), id(sound.GetId()),
		occluded(false), level(1) {}

	// state
	bool SpatialChannel::IsAudible() const
	{
		return level;
	}

	// update
	void SpatialChannel::DoUpdate(float deltaTime)
	{
		if (occluded)
			level = std::max(level - deltaTime / fadeOutDuration, 0.f);
	}

	// modifiers
	void SpatialChannel::Occlude()
	{
		occluded = true;
	}

	// sound access
	phys::Sound *SpatialChannel::GetSound() const
	{
		return util::GetPointerById<phys::Sound>(id);
	}
	util::Identifiable::Id SpatialChannel::GetId() const
	{
		return id;
	}

	// state
	float SpatialChannel::DoGetLinearVolume() const
	{
		phys::Sound *sound = GetSound();
		return sound ?
			math::DecibelToLinear(sound->GetVolume()) *
			math::HermiteScale(level, fadeExponent) : -math::Inf();
	}
}}
