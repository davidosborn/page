/**
 * @section license
 *
 * Copyright (c) 2006-2012 David Osborn
 *
 * Permission is granted to use and redistribute this software in source and
 * binary form, with or without modification, subject to the following
 * conditions:
 *
 * 1. Redistributions in source form must retain the above copyright notice,
 *    this list of conditions, and the following disclaimer.

 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions, and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution, and in the same
 *    place and form as other copyright, license, and disclaimer information.
 *
 * As a special exception, distributions of derivative works in binary form may
 * include an acknowledgement in place of the above copyright notice, this list
 * of conditions, and the following disclaimer in the documentation and/or other
 * materials provided with the distribution, and in the same place and form as
 * other acknowledgements, similar in substance to the following:
 *
 *    Portions of this software are based on the work of David Osborn.
 *
 * This software is provided "as is", without any express or implied warranty.
 * In no event will the authors be liable for any damages arising out of the use
 * of this software.
 */

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
