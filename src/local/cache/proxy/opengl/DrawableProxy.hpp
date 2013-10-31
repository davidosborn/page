#ifndef    page_local_cache_proxy_opengl_DrawableProxy_hpp
#   define page_local_cache_proxy_opengl_DrawableProxy_hpp

#	include "../../../phys/node/Form.hpp" // Form::Part
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
	class DrawableProxy : public BasicProxy<vid::opengl::Drawable>
	{
		IMPLEMENT_CLONEABLE(DrawableProxy, BasicProxy<vid::opengl::Drawable>)

		/*-------+
		| traits |
		+-------*/

		public:
		using typename BasicProxy<vid::opengl::Drawable>::pointer;

		/*-------------+
		| constructors |
		+-------------*/

		explicit DrawableProxy(const Proxy<res::Mesh> &);
		explicit DrawableProxy(const phys::Form::Part &);

		private:
		DrawableProxy(const Proxy<res::Mesh> &, util::Identifiable::Id);

		/*--------------------------+
		| BasicProxy implementation |
		+--------------------------*/

		pointer DoLock() const override;

		/*-------------+
		| data members |
		+-------------*/

		Proxy<res::Mesh> mesh;
		util::Identifiable::Id partId;
	};
}}}

#endif
