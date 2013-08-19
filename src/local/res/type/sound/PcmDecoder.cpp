#include <cassert>

#include "../../pipe/EndianPipe.hpp"
#include "PcmDecoder.hpp"
#include "PcmStream.hpp"

namespace page
{
	namespace res
	{
		/*--------------------------+
		| constructors & destructor |
		+--------------------------*/

		PcmDecoder::PcmDecoder(const std::shared_ptr<const Pipe> &pipe, unsigned channels, unsigned bitDepth, util::Endian endian) :
			pipe(pipe), sampleSize(channels * bitDepth / 8)
		{
			assert(pipe);
			if (endian != util::nativeEndian)
				switch (bitDepth)
				{
					case 8: break;
					case 16: this->pipe.reset(new EndianPipe(pipe, "w", endian)); break;
					case 24: this->pipe.reset(new EndianPipe(pipe, "t", endian)); break;
					case 32: this->pipe.reset(new EndianPipe(pipe, "d", endian)); break;
					case 48: this->pipe.reset(new EndianPipe(pipe, "T", endian)); break;
					case 64: this->pipe.reset(new EndianPipe(pipe, "q", endian)); break;
					default: assert(!"unsupported bit depth");
				}
		}

		/*-----------+
		| operations |
		+-----------*/

		std::unique_ptr<AudioStream> PcmDecoder::Open() const
		{
			return std::unique_ptr<AudioStream>(new PcmStream(*pipe, sampleSize));
		}
	}
}
