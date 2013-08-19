#include "../../err/platform/openal.hpp" // CheckError
#include "Buffer.hpp" // Buffer::{~Buffer,Update}, MakeBuffer
#include "Channel.hpp"

namespace page { namespace aud { namespace openal
{
	// construct/destroy
	Channel::Channel(const cache::Proxy<res::Sound> &sound, bool loop, float playPosition) :
		aud::Channel(playPosition)
	{
		alGenSources(1, &source);
		buffer.reset(MakeBuffer(source, sound, loop, playPosition));
		alSourcef(source, AL_GAIN, 0);
		alSourcePlay(source);
		err::openal::CheckError();
	}
	Channel::~Channel()
	{
		buffer.reset();
		alDeleteSources(1, &source);
	}

	// source access
	ALuint Channel::GetSource() const
	{
		return source;
	}

	// update
	void Channel::DoUpdate2()
	{
		alSourcef(source, AL_GAIN, GetLinearVolume());
		buffer->Update();
		DoUpdate3();
	}
}}}
