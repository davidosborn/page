#ifndef    page_local_res_type_Sound_hpp
#   define page_local_res_type_Sound_hpp

#	include <memory> // shared_ptr

namespace page
{
	namespace res
	{
		class AudioDecoder;

		struct Sound
		{
			unsigned channels, bitDepth, frequency, samples;
			std::shared_ptr<AudioDecoder> decoder;
		};

		// attributes
		unsigned GetSize(const Sound &);
		float GetDuration(const Sound &);
	}
}

#endif
