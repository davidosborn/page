/**
 * @copyright
 *
 * Copyright (c) 2006-2014 David Osborn
 *
 * Permission is granted to use and redistribute this software in source and
 * binary form, with or without modification, subject to the following
 * conditions:
 *
 * 1. Redistributions in source form must retain the above copyright notice,
 *    this list of conditions, and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions, and the following disclaimer in the same place
 *    and form as other copyright, license, and disclaimer information.
 *
 * 3. Redistributions in binary form must also include an acknowledgement in the
 *    same place and form as other acknowledgements (such as the credits),
 *    similar in substance to the following:
 *
 *       Portions of this software are based on the work of David Osborn.
 *
 * This software is provided "as is", without any express or implied warranty.
 * In no event will the authors be liable for any damages arising out of the use
 * of this software.
 */

#include "Encoder.hpp"

namespace page { namespace res { namespace clip
{
	/*-------------+
	| constructors |
	+-------------*/

	Encoder::Encoder(const Callback &cb, unsigned frameSize) :
		cb(cb), frameSize(frameSize) {}

	/*----------+
	| raw input |
	+----------*/

	void Encoder::Write(const void *s, unsigned n)
	{
		while (n)
		{
			unsigned n2 = std::min(n, frameSize - buffer.size());
			buffer.insert(buffer.end(),
				static_cast<const char *>(s),
				static_cast<const char *>(s) + n2);
			s = static_cast<const char *>(s) + n2;
			n -= n2;
			if (buffer.size() == frameSize)
			{
				Encode(&*buffer.begin());
				buffer.clear();
			}
		}
	}

	/*---------------+
	| encoded output |
	+---------------*/

	void Encoder::Return(const void *s, unsigned n) const
	{
		cb(s, n);
	}
}}}
