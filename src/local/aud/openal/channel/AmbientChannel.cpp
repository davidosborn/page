#include "../../Sound.hpp" // Sound::{Get{PlayPosition,Sound},IsLooping}
#include "AmbientChannel.hpp"

namespace page { namespace aud { namespace openal
{
	// construct
	AmbientChannel::AmbientChannel(const Sound &sound) :
		aud::Channel(sound.GetPlayPosition()),
		Channel(sound.GetSound(), sound.IsLooping(), sound.GetPlayPosition()),
		aud::AmbientChannel(sound)
	{
		alSourcef(GetSource(), AL_ROLLOFF_FACTOR, 0);
		alSourcei(GetSource(), AL_SOURCE_RELATIVE, AL_TRUE);
	}

	// update
	void AmbientChannel::DoUpdate3() {}
}}}
