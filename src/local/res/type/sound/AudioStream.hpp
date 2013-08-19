#ifndef    page_local_res_type_sound_AudioStream_hpp
#   define page_local_res_type_sound_AudioStream_hpp

#	include "../../../util/class/special_member_functions.hpp" // MAKE_UNCOPYABLE

namespace page
{
	namespace res
	{
		class AudioStream
		{
			/*--------------------------+
			| constructors & destructor |
			+--------------------------*/

			public:
			virtual ~AudioStream() = default;

			/*----------------------+
			| copy & move semantics |
			+----------------------*/

			MAKE_UNCOPYABLE(AudioStream)

			/*-----------+
			| operations |
			+-----------*/

			public:
			virtual unsigned Read(void *, unsigned) = 0;
			virtual void Seek(unsigned sample) = 0;
		};
	}
}

#endif
