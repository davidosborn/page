#ifndef    page_local_cache_proxy_opengl_DrawableProxy_hpp
#   define page_local_cache_proxy_opengl_DrawableProxy_hpp

#	include "../../../phys/Form.hpp" // Form::Part
#	include "../../../util/Identifiable.hpp" // Identifiable::Id
#	include "../BasicProxy.hpp"
#	include "../Proxy.hpp"

namespace page
{
	namespace res { class Mesh; }
	namespace vid { namespace opengl { class Drawable; }}
}

namespace page { namespace cache { namespace opengl
{
	/**
	 * A proxy representing an instance of vid::opengl::Drawable in the cache.
	 */
	class DrawableProxy :
		public BasicProxy<vid::opengl::Drawable>,
		public virtual util::Cloneable<DrawableProxy, BasicProxy<vid::opengl::Drawable>>
	{
		/*-------+
		| traits |
		+-------*/

		public:
		using BasicProxy<ALuint>::pointer;

		/*--------------------------+
		| constructors & destructor |
		+--------------------------*/

		explicit DrawableProxy(const Proxy<res::Mesh> &);
		explicit DrawableProxy(const phys::Form::Part &);

		/*--------------------------+
		| BasicProxy implementation |
		+--------------------------*/

		private:
		pointer DoLock() const override;

		/*-------------+
		| data members |
		+-------------*/

		Proxy<res::Mesh> mesh;
		util::Identifiable::Id partId;
	};
}}}

#endif
