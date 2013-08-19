#include "../../cache/proxy/Proxy.hpp"
#include "../../res/type/Sound.hpp" // GetSize
#include "Buffer.hpp"
#include "buffer/StaticBuffer.hpp"
#include "buffer/StreamBuffer.hpp"

namespace page { namespace aud { namespace openal
{
	// maximum size of audio data before streaming kicks in
	const unsigned maxStaticSize = 262144;

	// destroy
	Buffer::~Buffer() {}

	// factory function
	Buffer *MakeBuffer(ALuint source, const cache::Proxy<res::Sound> &sound, bool loop, float playPosition)
	{
		return GetSize(*sound) <= maxStaticSize ?
			static_cast<Buffer *>(new StaticBuffer(source, sound, loop, playPosition)) :
			static_cast<Buffer *>(new StreamBuffer(source, *sound, loop, playPosition));
	}
}}}
