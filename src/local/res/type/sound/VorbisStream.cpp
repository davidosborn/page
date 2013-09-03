#include "../../../err/Exception.hpp"
#include "../../adapt/vorbis.hpp" // CheckError, Open
#include "VorbisStream.hpp"

namespace page
{
	namespace res
	{
		/*-------------+
		| constructors |
		+-------------*/

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
