#ifndef    page_local_cache_proxy_opengl_FontTexture_hpp
#   define page_local_cache_proxy_opengl_FontTexture_hpp

#	include <GL/gl.h> // GLuint

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
	class FontTexture :
		public Proxy<vid::opengl::FontTexture>,
		public virtual util::Cloneable<FontTexture, Proxy<vid::opengl::FontTexture>>
	{
		/*------+
		| types |
		+------*/

		public:
		typedef typename Proxy<vid::opengl::FontTexture>::Instance Instance;

		/*--------------------------+
		| constructors & destructor |
		+--------------------------*/

		public:
		explicit FontTexture(const Proxy<res::Font> &, unsigned fontSize);

		/*----------+
		| observers |
		+----------*/

		public:
		std::string GetType() const override;
		std::string GetSource() const override;
		operator bool() const override;

		/*--------------+
		| instantiation |
		+--------------*/

		private:
		Instance Make() const override;

		/*-------------+
		| data members |
		+-------------*/

		private:
		Proxy<res::Font> font;
		unsigned fontSize;
	};
}}}

#endif
