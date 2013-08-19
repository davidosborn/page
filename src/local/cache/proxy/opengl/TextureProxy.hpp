#ifndef    page_local_cache_proxy_opengl_Texture_hpp
#   define page_local_cache_proxy_opengl_Texture_hpp

#	include "../../../math/Vector.hpp"
#	include "../../../vid/opengl/TextureFlags.hpp"
#	include "../../../vid/opengl/TextureFormat.hpp"
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
	class Texture :
		public Proxy<vid::opengl::Texture>,
		public virtual util::Cloneable<Texture, Proxy<vid::opengl::Texture>>
	{
		/*------+
		| types |
		+------*/

		public:
		typedef typename Proxy<vid::opengl::Texture>::Instance Instance;

		/*--------------------------+
		| constructors & destructor |
		+--------------------------*/

		public:
		explicit Texture(const Proxy<res::Image> &,
			vid::opengl::TextureFormat = vid::opengl::defaultTextureFormat,
			vid::opengl::TextureFlags =
				static_cast<vid::opengl::TextureFlags>(
					vid::opengl::filterTextureFlag |
					vid::opengl::mipmapTextureFlag),
			const math::Vector<2, bool> &clamp = false);

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
		Proxy<res::Image> image;
		vid::opengl::TextureFormat format;
		vid::opengl::TextureFlags flags;
		math::Vector<2, bool> clamp;
	};
}}}

#endif
