/**
 * @section license
 *
 * Copyright (c) 2006-2013 David Osborn
 *
 * Permission is granted to use and redistribute this software in source and
 * binary form, with or without modification, subject to the following
 * conditions:
 *
 * 1. Redistributions in source form must retain the above copyright notice,
 *    this list of conditions, and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions, and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution, and in the same
 *    place and form as other copyright, license, and disclaimer information.
 *
 * As a special exception, distributions of derivative works in binary form may
 * include an acknowledgement in place of the above copyright notice, this list
 * of conditions, and the following disclaimer in the documentation and/or other
 * materials provided with the distribution, and in the same place and form as
 * other acknowledgements, similar in substance to the following:
 *
 *    Portions of this software are based on the work of David Osborn.
 *
 * This software is provided "as is", without any express or implied warranty.
 * In no event will the authors be liable for any damages arising out of the use
 * of this software.
 */

#ifndef    page_local_cache_proxy_opengl_Drawable_hpp
#   define page_local_cache_proxy_opengl_Drawable_hpp

#	include "../../../phys/Form.hpp" // Form::Part
#	include "../../../util/Identifiable.hpp" // Identifiable::Id
#	include "../../../util/raii/copy_ptr.hpp"
#	include "../Proxy.hpp"

namespace page
{
	namespace res { class Mesh; }
	namespace vid { namespace opengl { class Drawable; }}
}

namespace page { namespace cache { namespace opengl
{
	/**
	 * A proxy representing an instance of @c vid::opengl::Drawable in the
	 * cache.
	 */
	class Drawable :
		public Proxy<vid::opengl::Drawable>,
		public virtual util::Cloneable<Drawable, Proxy<vid::opengl::Drawable>>
	{
		/*------+
		| types |
		+------*/

		public:
		typedef typename Proxy<vid::opengl::Drawable>::Instance Instance;

		/*--------------------------+
		| constructors & destructor |
		+--------------------------*/

		public:
		explicit Drawable(const Proxy<res::Mesh> &);
		explicit Drawable(const phys::Form::Part &);

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
		util::copy_ptr<Proxy<res::Mesh>> mesh;
		util::Identifiable::Id partId;
	};
}}}

#endif
