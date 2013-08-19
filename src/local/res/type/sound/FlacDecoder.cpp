#include <cassert>

#include "FlacDecoder.hpp"
#include "FlacStream.hpp"

namespace page
{
	namespace res
	{
		/*--------------------------+
		| constructors & destructor |
		+--------------------------*/

		FlacDecoder::FlacDecoder(const std::shared_ptr<const Pipe> &pipe) :
			pipe(pipe)
		{
			assert(pipe);
		}

		/*-----------+
		| operations |
		+-----------*/

		std::unique_ptr<AudioStream> FlacDecoder::Open() const
		{
			return std::unique_ptr<AudioStream>(new FlacStream(*pipe));
		}

		/*------------+
		| constraints |
		+------------*/

		unsigned FlacDecoder::LimitChannels(unsigned channels)
		{
			// either mono or stereo
			return 1 + (channels >= 2);
		}

		unsigned FlacDecoder::LimitBitDepth(unsigned bitDepth)
		{
			// expand to byte alignment
			return bitDepth + 7 & (~0 << 3);
		}
	}
}
