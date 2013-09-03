#ifndef    page_local_res_type_sound_AudioDecoder_hpp
#   define page_local_res_type_sound_AudioDecoder_hpp

#	include <memory> // unique_ptr

namespace page
{
	namespace res
	{
		class AudioStream;

		class AudioDecoder
		{
			/*-------------+
			| constructors |
			+-------------*/

			public:
			virtual ~AudioDecoder() = default;

			/*-----------+
			| operations |
			+-----------*/

			public:
			virtual std::unique_ptr<AudioStream> Open() const = 0;
		};
	}
}

#endif
