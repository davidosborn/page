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
	class FontTextureProxy :
		public BasicProxy<vid::opengl::FontTexture>,
		public virtual util::Cloneable<FontTextureProxy, BasicProxy<vid::opengl::FontTexture>>
	{
		/*-------+
		| traits |
		+-------*/

		public:
		using BasicProxy<vid::opengl::FontTexture>::pointer;

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
