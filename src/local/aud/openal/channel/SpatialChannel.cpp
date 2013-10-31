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
