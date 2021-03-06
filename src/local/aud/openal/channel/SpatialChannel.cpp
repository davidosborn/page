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

#include <cassert>

#include "../../../math/float.hpp" // DecibelToLinear
#include "../../../math/Vector.hpp"
#include "../../../phys/node/Sound.hpp" // Sound::Get{IsLooping,Orientation,{,Play}Position,Sound,Velocity}
#include "SpatialChannel.hpp"

namespace page { namespace aud { namespace openal
{
	// construct
	SpatialChannel::SpatialChannel(const phys::Sound &sound) :
		aud::Channel(sound.GetPlayPosition()),
		Channel(sound.GetSound(), sound.IsLooping(), sound.GetPlayPosition()),
		aud::SpatialChannel(sound)
	{
		DoUpdate3();
	}
	SpatialChannel::~SpatialChannel()
	{
		const phys::Sound *sound(GetSound());
		// FIXME: waiting for implementation
		//if (sound) sound->Stop();
	}

	// update
	void SpatialChannel::DoUpdate3()
	{
		const phys::Sound *sound(GetSound());
		assert(sound);
		alSourcefv(GetSource(), AL_POSITION, &*math::Vector<3, ALfloat>(sound->GetPosition()).begin());
		// FIXME: waiting for implementation
		/*alSourcefv(GetSource(), AL_VELOCITY, &*math::Vector<3, ALfloat>(sound->GetVelocity()).begin());
		alSourcefv(GetSource(), AL_DIRECTION, &*math::Vector<3, ALfloat>(
			math::NormVector<3>() * sound->GetOrientation()).begin());*/
	}
}}}
