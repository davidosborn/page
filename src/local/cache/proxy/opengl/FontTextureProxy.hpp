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

#ifndef    page_local_cache_proxy_opengl_FontTextureProxy_hpp
#   define page_local_cache_proxy_opengl_FontTextureProxy_hpp

#	include <GL/gl.h> // GLuint

#	include "../BasicProxy.hpp"
#	include "../Proxy.hpp"

namespace page
{
	namespace res { class Font; }
	namespace vid { namespace opengl { class FontTexture; }}
}

namespace page { namespace cache { namespace opengl
{
	/**
	 * A proxy representing a font texture in the cache.
	 */
	class FontTextureProxy : public BasicProxy<vid::opengl::FontTexture>
	{
		IMPLEMENT_CLONEABLE(FontTextureProxy, BasicProxy<vid::opengl::FontTexture>)

		/*-------+
		| traits |
		+-------*/

		public:
		using typename BasicProxy<vid::opengl::FontTexture>::pointer;

		/*-------------+
		| constructors |
		+-------------*/

		explicit FontTexture(const Proxy<res::Font> &, unsigned fontSize);

		/*--------------------------+
		| BasicProxy implementation |
		+--------------------------*/

		private:
		pointer DoLock() const override;

		/*-------------+
		| data members |
		+-------------*/

		Proxy<res::Font> font;
		unsigned fontSize;
	};
}}}

#endif
