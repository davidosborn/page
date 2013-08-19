#ifndef    page_local_aud_openal_buffer_StreamBuffer_hpp
#   define page_local_aud_openal_buffer_StreamBuffer_hpp

#	include <array>
#	include <memory> // unique_ptr
#	include <vector>

#	include "../Buffer.hpp"

namespace page { namespace res { class AudioStream; }}

namespace page { namespace aud { namespace openal
{
	struct StreamBuffer : Buffer
	{
		// construct/destroy
		StreamBuffer(ALuint source, const res::Sound &, bool loop, float playPosition);
		~StreamBuffer();

		// update
		void Update();

		private:
		// buffering
		void Queue(const ALuint *buffers, unsigned n);

		ALuint source;
		std::unique_ptr<res::AudioStream> stream;
		ALenum format;
		unsigned frequency;
		bool loop, end;
		typedef std::array<ALuint, 2> Buffers;
		Buffers buffers;
		typedef std::vector<char> BufferData;
		BufferData bufferData;
	};
}}}

#endif
