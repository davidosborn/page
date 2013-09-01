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
	class TextureProxy :
		public BasicProxy<vid::opengl::Texture>,
		public virtual util::Cloneable<TextureProxy, BasicProxy<vid::opengl::Texture>>
	{
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
