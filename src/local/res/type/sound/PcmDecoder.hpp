#ifndef    page_local_res_type_sound_PcmDecoder_hpp
#   define page_local_res_type_sound_PcmDecoder_hpp

#	include <memory> // shared_ptr

#	include "../../../util/endian.hpp" // Endian
#	include "AudioDecoder.hpp"

namespace page
{
	namespace res
	{
		class Pipe;

		class PcmDecoder : public AudioDecoder
		{
			/*--------------------------+
			| constructors & destructor |
			+--------------------------*/

			public:
			PcmDecoder(
				const std::shared_ptr<const Pipe> &,
				unsigned channels,
				unsigned bitDepth,
				util::Endian = util::nativeEndian);

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
			unsigned sampleSize;
		};
	}
}

#endif
