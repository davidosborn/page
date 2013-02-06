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

#include "../../../err/Exception.hpp"
#include "../../adapt/vorbis.hpp" // CheckError, Open
#include "VorbisStream.hpp"

namespace page
{
	namespace res
	{
		/*--------------------------+
		| constructors & destructor |
		+--------------------------*/

		VorbisStream::VorbisStream(const Pipe &pipe) : vf(vorbis::Open(pipe))
		{
			if (!vf)
				THROW((err::Exception<err::ResModuleTag, err::VorbisPlatformTag, err::FormatTag>()))
			if (!(vi = ov_info(vf.get(), -1)))
				THROW((err::Exception<err::ResModuleTag, err::VorbisPlatformTag>()))
		}

		/*-----------+
		| operations |
		+-----------*/

		unsigned VorbisStream::Read(void *s, unsigned n)
		{
			unsigned i = 0;
			while (i != n)
			{
				int bitstream;
				long result = ov_read(vf.get(),
					reinterpret_cast<char *>(s), n - i,
#ifdef WORDS_BIG_ENDIAN
					1,
#else
					0,
#endif
					2, 1, &bitstream);
				vorbis::CheckError(result);
				if (!result) break;
				s = reinterpret_cast<char *>(s) + result;
				i += result;
			}
			return i;
		}

		void VorbisStream::Seek(unsigned sample)
		{
			vorbis::CheckError(ov_pcm_seek(vf.get(), sample));
		}
	}
}
