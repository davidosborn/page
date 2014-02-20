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
