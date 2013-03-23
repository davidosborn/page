/**
 * @section license
 *
 * Copyright (c) 2006-2013 David Osborn
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

#ifndef    page_local_vid_opengl_Texture_hpp
#   define page_local_vid_opengl_Texture_hpp

#	include <GL/gl.h> // GLuint

#	include "../../math/fwd.hpp" // RgbaColor
#	include "../../math/Vector.hpp"
#	include "../../util/class/Uncopyable.hpp"
#	include "TextureFlags.hpp"
#	include "TextureFormat.hpp"

namespace page
{
	namespace res { class Image; }

	namespace vid
	{
		namespace opengl
		{
			struct Texture : util::Uncopyable<Texture>
			{
				typedef TextureFlags  Flags;
				typedef TextureFormat Format;

				// construct/destroy
				explicit Texture(const math::RgbaColor<> &);
				explicit Texture(const res::Image &,
					Format = defaultTextureFormat,
					Flags = static_cast<Flags>(filterTextureFlag | mipmapTextureFlag),
					const math::Vector<2, bool> &clamp = false);
				~Texture();

				// attributes
				const math::Vector<2, unsigned> &GetSize() const;
				const math::Vector<2, unsigned> &GetPow2Size() const;

				// handle access
				GLuint GetHandle() const;

				private:
				GLuint handle;
				math::Vector<2, unsigned> size, pow2Size;
			};

			// binding
			void Bind(const Texture &);
		}
	}
}

#endif
