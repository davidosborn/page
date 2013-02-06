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
