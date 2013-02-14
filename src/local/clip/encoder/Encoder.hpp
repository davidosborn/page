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
 *
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

#ifndef    page_local_clip_encoder_Encoder_hpp
#   define page_local_clip_encoder_Encoder_hpp

	// C++
#	include <functional> // function
#	include <vector>

	// local
#	include "../../math/fwd.hpp"
#	include "../../util/class.hpp" // MAKE_UNCOPYABLE
#	include "EncoderFactory.hpp"

namespace page
{
	namespace clip
	{
		class Encoder
		{
			/*------+
			| types |
			+------*/

			public:
			/**
			 *
			 */
			typedef std::function<void (const void *, unsigned)> Callback;

			/**
			 *
			 */
			typedef EncoderFactory Factory;

			/*--------------------------+
			| constructors & destructor |
			+--------------------------*/

			protected:
			Encoder(const Callback &, unsigned frameSize);

			public:
			virtual ~Encoder() = default;

			/*----------------------+
			| copy & move semantics |
			+----------------------*/

			MAKE_UNCOPYABLE(Encoder)

			/*----------+
			| raw input |
			+----------*/

			public:
			/**
			 *
			 *
			 * @note Expects packed RGB8 data.
			 */
			void Write(const void *, unsigned);

			/*---------------+
			| encoded output |
			+---------------*/

			protected:
			/**
			 *
			 */
			void Return(const void *, unsigned) const;

			/*---------+
			| encoding |
			+---------*/

			private:
			/**
			 *
			 *
			 * @note Expects one complete frame of data.
			 */
			virtual void Encode(const void *) = 0;

			/*-------------+
			| data members |
			+-------------*/

			private:
			/**
			 *
			 */
			Callback cb;

			/**
			 *
			 */
			unsigned frameSize;

			/**
			 *
			 */
			std::vector<char> buffer;
		};
	}
}

#endif
