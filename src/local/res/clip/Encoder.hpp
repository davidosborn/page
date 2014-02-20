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

#ifndef    page_local_res_clip_Encoder_hpp
#   define page_local_res_clip_Encoder_hpp

#	include <vector>

#	include "../../util/class/special_member_functions.hpp" // Uncopyable
#	include "EncoderRegistry.hpp"

namespace page { namespace res { namespace clip
{
	/**
	 * The base class for a clip encoder.
	 */
	class Encoder : public util::Uncopyable<Encoder>
	{
		/*-------------+
		| constructors |
		+-------------*/

		protected:
		Encoder(const EncoderCallback &, unsigned frameSize);

		public:
		virtual ~Encoder() = default;

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
}}}

#endif
