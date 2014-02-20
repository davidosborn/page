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

// image resource
// sub-byte channels use MSB to LSB order
// multi-byte channels use native byte order

#ifndef    page_local_res_type_Image_hpp
#   define page_local_res_type_Image_hpp

#	include <vector>

#	include "../../math/Vector.hpp"
#	include "../../util/AlignedAllocator.hpp"

namespace page
{
	namespace res
	{
		struct Image
		{
			math::Vec2u size;
			struct Channel
			{
				enum Type
				{
					null  = 0,
					red   = 0x0001,
					green = 0x0002,
					blue  = 0x0004,
					alpha = 0x0008,
					gray  = red | green | blue,
					mono  = red | green | blue | alpha
				};

				// construct
				Channel(Type = null, unsigned depth = 0);

				Type type;
				unsigned depth;
			};
			typedef std::vector<Channel> Channels;
			Channels channels;
			unsigned alignment;
			// NOTE: 128-bit alignment for SIMD support
			typedef std::vector<unsigned char, util::AlignedAllocator<unsigned char, 16>> Data;
			Data data;
		};

		// image comparison
		bool operator ==(const Image &, const Image &);
		bool operator !=(const Image &, const Image &);

		// channel comparison
		bool operator ==(const Image::Channel &, const Image::Channel &);
		bool operator !=(const Image::Channel &, const Image::Channel &);

		// channel type
		bool HasColor(const Image::Channels &);
		bool HasAlpha(const Image::Channels &);

		// channel depth
		unsigned GetDepth(const Image::Channels &);
		unsigned GetComponentDepth(const Image::Channels &);
		unsigned GetColorDepth(const Image::Channels &);
		unsigned GetAlphaDepth(const Image::Channels &);

		// transformation
		Image Flip(const Image &, const math::Vector<2, bool> &);
		Image Scale(const Image &, const math::Vec2u &);

		// format conversion
		Image Convert(const Image &, const Image::Channels &);
		Image Convert(const Image &, const Image::Channels &, unsigned alignment);

		// alignment
		bool IsAligned(const Image &, unsigned alignment);
		Image Align(const Image &, unsigned alignment);
	}
}

#endif
