#ifndef    page_local_res_type_sound_FlacDecoder_hpp
#   define page_local_res_type_sound_FlacDecoder_hpp

#	include <memory> // shared_ptr

#	include "AudioDecoder.hpp"

namespace page
{
	namespace res
	{
		class Pipe;

		class FlacDecoder : public AudioDecoder
		{
			/*--------------------------+
			| constructors & destructor |
			+--------------------------*/

			public:
			explicit FlacDecoder(const std::shared_ptr<const Pipe> &);

			/*-----------+
			| operations |
			+-----------*/

			public:
			std::unique_ptr<AudioStream> Open() const override;

			/*------------+
			| constraints |
			+------------*/

			public:
			static unsigned LimitChannels(unsigned);
			static unsigned LimitBitDepth(unsigned);

			/*-------------+
			| data members |
			+-------------*/

			private:
			std::shared_ptr<const Pipe> pipe;
		};
	}
}

#endif
