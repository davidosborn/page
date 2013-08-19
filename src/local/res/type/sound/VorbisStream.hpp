#ifndef    page_local_res_type_sound_VorbisStream_hpp
#   define page_local_res_type_sound_VorbisStream_hpp

#	include <vorbis/vorbisfile.h> // vorbis_info

#	include "../../adapt/vorbis.hpp" // File
#	include "AudioStream.hpp"

namespace page
{
	namespace res
	{
		class Pipe;

		class VorbisStream : public AudioStream
		{
			/*--------------------------+
			| constructors & destructor |
			+--------------------------*/

			public:
			VorbisStream(const Pipe &);

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
			vorbis::File vf;
			vorbis_info *vi;
		};
	}
}

#endif
