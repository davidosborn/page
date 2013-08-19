#include "Encoder.hpp"

namespace page { namespace res { namespace clip
{
	/*--------------------------+
	| constructors & destructor |
	+--------------------------*/

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
