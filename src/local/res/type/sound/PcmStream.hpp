#ifndef    page_local_res_type_sound_PcmStream_hpp
#   define page_local_res_type_sound_PcmStream_hpp

#	include <memory> // unique_ptr

#	include "AudioStream.hpp"

namespace page
{
	namespace res
	{
		class Pipe;

		class PcmStream : public AudioStream
		{
			/*--------------------------+
			| constructors & destructor |
			+--------------------------*/

			public:
			PcmStream(const Pipe &, unsigned sampleSize);

			/*-----------+
			| operations |
			+-----------*/

			public:
			unsigned Read(void *, unsigned) override;
			void Seek(unsigned sample) override;

			/*-------------+
			| data members |
			+-------------*/

			private:
			std::unique_ptr<Stream> super;
			unsigned sampleSize;
		};
	}
}

#endif
