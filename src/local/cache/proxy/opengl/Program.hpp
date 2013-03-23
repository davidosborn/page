/**
 * @section license
 *
 * Copyright (c) 2006-2012 David Osborn
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

#ifndef    page_local_cache_proxy_opengl_Program_hpp
#   define page_local_cache_proxy_opengl_Program_hpp

#	include <vector>

#	include "../../../util/raii/copy_ptr.hpp"
#	include "../../../util/type_traits/container.hpp" // is_range
#	include "../../../util/type_traits/iterator.hpp" // is_iterator
#	include "../../../util/type_traits/sfinae.hpp" // ENABLE_IF
#	include "../Proxy.hpp"

namespace page
{
	namespace res { namespace opengl { class Shader; }}
	namespace vid { namespace opengl { class Program; }}
}

namespace page { namespace cache { namespace opengl
{
	/**
	 * A proxy representing a shader program in the cache.
	 */
	class Program :
		public Proxy<vid::opengl::Program>,
		public virtual util::Cloneable<Program, Proxy<vid::opengl::Program>>
	{
		/*------+
		| types |
		+------*/

		public:
		typedef typename Proxy<vid::opengl::Program>::Instance Instance;

		/*--------------------------+
		| constructors & destructor |
		+--------------------------*/

		public:
		/**
		 * Creates a proxy for the program from a range of shaders.
		 */
		template <typename ShaderInputRange>
			explicit Program(
				ShaderInputRange,
				ENABLE_IF((util::is_range<ShaderInputRange>::value)));

		/**
		 * Creates a proxy for the program from a range of shaders.
		 */
		template <typename ShaderInputIterator>
			Program(
				ShaderInputIterator first,
				ShaderInputIterator last,
				ENABLE_IF((util::is_iterator<ShaderInputIterator>::value)));

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

		/*---------------+
		| initialization |
		+---------------*/

		private:
		void PostInit();

		/*-------------+
		| data members |
		+-------------*/

		private:
		std::vector<util::copy_ptr<Proxy<res::opengl::Shader>>> shaders;
	};
}}}

#	include "Program.tpp"
#endif
