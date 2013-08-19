#ifndef    page_local_res_type_sound_VorbisDecoder_hpp
#   define page_local_res_type_sound_VorbisDecoder_hpp

#	include <memory> // shared_ptr

#	include "AudioDecoder.hpp"

namespace page
{
	namespace res
	{
		class Pipe;

		class VorbisDecoder : public AudioDecoder
		{
			/*--------------------------+
			| constructors & destructor |
			+--------------------------*/

			public:
			explicit VorbisDecoder(const std::shared_ptr<const Pipe> &);

			/*-----------+
			| operations |
			+-----------*/

			public:
			std::unique_ptr<AudioStream> Open() const override;

			/*-------------+
			| data members |
			+-------------*/

			private:
			std::shared_ptr<const Pipe> pipe;
		};
	}
}

#endif
