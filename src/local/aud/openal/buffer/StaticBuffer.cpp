#include <cassert>

#include "../../../cache/proxy/openal/Buffer.hpp"
#include "../../../res/type/Sound.hpp" // GetDuration
#include "StaticBuffer.hpp"

namespace page { namespace aud { namespace openal
{
	// construct/destroy
	StaticBuffer::StaticBuffer(ALuint source, const util::resource_ptr<res::Sound> &sound, bool loop, float playPosition) :
		source(source), buffer(cache::openal::Buffer(sound).Copy()),
		lock(buffer.lock())
	{
		alSourcei(source, AL_BUFFER, *lock);
		alSourcei(source, AL_LOOPING, loop);
		// FIXME: consider using an extension mechanism similar to that
		// of the OpenGL video module
		if (playPosition && alIsExtensionPresent("AL_EXT_OFFSET"))
		{
			assert(playPosition <= GetDuration(*sound));
			alSourcef(source, AL_SEC_OFFSET, playPosition);
		}
	}
	StaticBuffer::~StaticBuffer()
	{
		alSourceStop(source);
		alSourcei(source, AL_BUFFER, 0);
	}

	// update
	void StaticBuffer::Update() {}
}}}
