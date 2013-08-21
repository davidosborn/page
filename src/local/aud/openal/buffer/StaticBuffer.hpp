#ifndef    page_local_aud_openal_buffer_StaticBuffer_hpp
#   define page_local_aud_openal_buffer_StaticBuffer_hpp

#	include "../../../cache/proxy/Proxy.hpp"
#	include "../Buffer.hpp"

namespace page { namespace aud { namespace openal
{
	struct StaticBuffer : Buffer
	{
		// construct/destroy
		StaticBuffer(ALuint source, const cache::Proxy<res::Sound> &, bool loop, float playPosition);
		~StaticBuffer();

		// update
		void Update();

		private:
		ALuint source;
		cache::Proxy<ALuint> buffer;
		decltype(buffer.lock()) lock;
	};
}}}

#endif
