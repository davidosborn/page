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

#ifndef    page_local_cache_proxy_opengl_TextureProxy_hpp
#   define page_local_cache_proxy_opengl_TextureProxy_hpp

#	include "../../../math/Vector.hpp"
#	include "../../../vid/opengl/TextureFlags.hpp"
#	include "../../../vid/opengl/TextureFormat.hpp"
#	include "../BasicProxy.hpp"
#	include "../Proxy.hpp"

namespace page
{
	namespace res { class Image; }
	namespace vid { namespace opengl { class Texture; }}
}

namespace page { namespace cache { namespace opengl
{
	/**
	 * A proxy representing a texture in the cache.
	 */
	class TextureProxy : public BasicProxy<vid::opengl::Texture>
	{
		IMPLEMENT_CLONEABLE(TextureProxy, BasicProxy<vid::opengl::Texture>)

		/*-------+
		| traits |
		+-------*/

		public:
		using typename BasicProxy<vid::opengl::Texture>::pointer;

		/*-------------+
		| constructors |
		+-------------*/

		explicit TextureProxy(
			const Proxy<res::Image> &image,
			vid::opengl::TextureFormat format = vid::opengl::defaultTextureFormat,
			vid::opengl::TextureFlags flags =
				static_cast<vid::opengl::TextureFlags>(
					vid::opengl::filterTextureFlag |
					vid::opengl::mipmapTextureFlag),
			const math::Vector<2, bool> &clamp = false);

		private:
		/**
		 * Generates the appropriate signature for the constructor arguments.
		 */
		static Signature MakeSignature(
			const Proxy<res::Image> &image,
			vid::opengl::TextureFormat format,
			vid::opengl::TextureFlags flags,
			const math::Vector<2, bool> &clamp);

		/*--------------------------+
		| BasicProxy implementation |
		+--------------------------*/

		pointer DoLock() const override;

		/*-------------+
		| data members |
		+-------------*/

		Proxy<res::Image> image;
		vid::opengl::TextureFormat format;
		vid::opengl::TextureFlags flags;
		math::Vector<2, bool> clamp;
	};
}}}

#endif
