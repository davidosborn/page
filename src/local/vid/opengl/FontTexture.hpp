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

#ifndef    page_local_vid_opengl_FontTexture_hpp
#   define page_local_vid_opengl_FontTexture_hpp

#	include <unordered_map>

#	include <GL/gl.h> // GLuint

#	include "../../math/Aabb.hpp"

namespace page
{
	namespace res { class Font; }

	namespace vid
	{
		namespace opengl
		{
			struct FontTexture
			{
				// construct/destroy
				FontTexture(const res::Font &, unsigned fontSize);
				~FontTexture();

				// sections
				const math::Aabb<2> *GetSection(char) const;

				// handle access
				GLuint GetHandle() const;

				private:
				mutable GLuint handle;
				typedef std::unordered_map<char, math::Aabb<2>> Sections;
				Sections sections;
			};

			// binding
			void Bind(const FontTexture &);
		}
	}
}

#endif
