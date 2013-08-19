#ifndef    page_local_aud_openal_buffer_StaticBuffer_hpp
#   define page_local_aud_openal_buffer_StaticBuffer_hpp

#	include "../../../util/raii/resource_ptr.hpp"
#	include "../Buffer.hpp"

namespace page { namespace aud { namespace openal
{
	struct StaticBuffer : Buffer
	{
		// construct/destroy
		StaticBuffer(ALuint source, const util::resource_ptr<res::Sound> &, bool loop, float playPosition);
		~StaticBuffer();

		// update
		void Update();

		private:
		ALuint source;
		util::resource_ptr<ALuint> buffer;
		decltype(buffer::shared_ptr) lock;
	};
}}}

#endif
