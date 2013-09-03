#include <cassert>

#include "VorbisDecoder.hpp"
#include "VorbisStream.hpp"

namespace page
{
	namespace res
	{
		/*-------------+
		| constructors |
		+-------------*/

		VorbisDecoder::VorbisDecoder(const std::shared_ptr<const Pipe> &pipe) :
			pipe(pipe)
		{
			assert(pipe);
		}

		/*-----------+
		| operations |
		+-----------*/

		std::unique_ptr<AudioStream> VorbisDecoder::Open() const
		{
			return std::unique_ptr<AudioStream>(new VorbisStream(*pipe));
		}
	}
}
