#include "../../pipe/Pipe.hpp" // Pipe::Open
#include "../../pipe/Stream.hpp" // Stream::{ReadSome,Seek}
#include "PcmStream.hpp"

namespace page
{
	namespace res
	{
		/*--------------------------+
		| constructors & destructor |
		+--------------------------*/

		PcmStream::PcmStream(const Pipe &pipe, unsigned sampleSize) :
			super(pipe.Open()), sampleSize(sampleSize) {}

		/*-----------+
		| operations |
		+-----------*/

		unsigned PcmStream::Read(void *s, unsigned n)
		{
			return super->ReadSome(s, n);
		}

		void PcmStream::Seek(unsigned sample)
		{
			super->Seek(sample * sampleSize);
		}
	}
}
