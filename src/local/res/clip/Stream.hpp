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

#ifndef    page_local_res_clip_Stream_hpp
#   define page_local_res_clip_Stream_hpp

#	include <fstream> // ofstream
#	include <memory> // unique_ptr
#	include <string>
#	include <vector>

#	include "../../math/Vector.hpp"
#	include "../../util/class/special_member_functions.hpp" // Uncopyable

namespace page { namespace res { class Image; }}

namespace page { namespace res { namespace clip
{
	class Encoder;

	/**
	 * An output stream for encoding and writing a clip to a file.
	 *
	 * @fixme This class could be made copyable by storing the encoder in a @c
	 *        std::shared_ptr, which would only make sense if the encoder didn't
	 *        maintain any output state.
	 */
	struct Stream : public util::Uncopyable<Stream>
	{
		/*-------------+
		| constructors |
		+-------------*/

		public:
		Stream(
			const std::string &path,
			const std::string &format,
			const math::Vec2u &size,
			float frameRate,
			float quality);

		~Stream();

		/*-----------+
		| attributes |
		+-----------*/

		public:
		const math::Vec2u &GetSize() const;

		/*------------------+
		| audio/video input |
		+------------------*/

		public:
		void Write(const res::Image &);

		/*---------------+
		| encoded output |
		+---------------*/

		private:
		void WriteEncoded(const void *, unsigned);

		/*-------------+
		| data members |
		+-------------*/

		private:
		math::Vec2u size;
		std::ofstream fs;
		std::unique_ptr<Encoder> encoder;
	};
}}}

#endif
