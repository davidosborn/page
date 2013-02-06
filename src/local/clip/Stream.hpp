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

#ifndef    page_local_clip_Stream_hpp
#   define page_local_clip_Stream_hpp

	// C++
#	include <memory> // unique_ptr
#	include <string>

	// Boost
#	include <boost/filesystem/fstream.hpp> // ofstream
#	include <boost/filesystem/path.hpp>

	// local
#	include "../math/Vector.hpp"
#	include "../util/NonCopyable.hpp"

namespace page
{
	namespace res { class Image; }

	namespace clip
	{
		class Encoder;

		struct Stream : util::NonCopyable
		{
			// construct/destroy
			Stream(
				const boost::filesystem::path &path,
				const std::string &format,
				const math::Vector<2, unsigned> &size,
				float frameRate, float quality);
			~Stream();

			// attributes
			const math::Vector<2, unsigned> &GetSize() const;

			// audio/video input
			void Write(const res::Image &);

			private:
			// encoded output
			void WriteEncoded(const void *, unsigned);

			math::Vector<2, unsigned> size;
			boost::filesystem::ofstream fs;
			std::unique_ptr<Encoder> encoder;
		};
	}
}

#endif
