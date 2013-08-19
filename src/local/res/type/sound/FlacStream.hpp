#ifndef    page_local_res_type_sound_FlacStream_hpp
#   define page_local_res_type_sound_FlacStream_hpp

#	include <memory> // shared_ptr
#	include <vector>

#	include <FLAC/stream_decoder.h>

#	include "AudioStream.hpp"

namespace page
{
	namespace res
	{
		class Pipe;

		class FlacStream : public AudioStream
		{
			/*--------------------------+
			| constructors & destructor |
			+--------------------------*/

			public:
			explicit FlacStream(const Pipe &);

			/*-----------+
			| operations |
			+-----------*/

			public:
			unsigned Read(void *, unsigned) override;
			void Seek(unsigned sample) override;

			/*---------------+
			| implementation |
			+---------------*/

			private:
			/**
			 * Write callback.
			 */
			void Write(const FLAC__StreamDecoder *, const FLAC__Frame *, const FLAC__int32 *const buffers[]);

			/*-------------+
			| data members |
			+-------------*/

			private:
			bool init;
			unsigned channels, bitDepth;
			std::vector<char> buffer;
			// NOTE: FLAC__StreamDecoder must be initialized after any
			// variables that are used by the Write callback
			std::shared_ptr<FLAC__StreamDecoder> sd;
		};
	}
}

#endif
