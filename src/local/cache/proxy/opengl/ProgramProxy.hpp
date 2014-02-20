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

#ifndef    page_local_cache_proxy_opengl_ProgramProxy_hpp
#   define page_local_cache_proxy_opengl_ProgramProxy_hpp

#	include <vector>

#	include "../../../util/type_traits/iterator.hpp" // is_iterator
#	include "../../../util/type_traits/range.hpp" // is_range
#	include "../../../util/type_traits/sfinae.hpp" // ENABLE_IF
#	include "../BasicProxy.hpp"
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
	class ProgramProxy : public BasicProxy<vid::opengl::Program>
	{
		IMPLEMENT_CLONEABLE(ProgramProxy, BasicProxy<vid::opengl::Program>)

		/*-------+
		| traits |
		+-------*/

		public:
		using typename BasicProxy<vid::opengl::Program>::pointer;

		/*-------------+
		| constructors |
		+-------------*/

		/**
		 * Creates a program proxy from a range of shaders.
		 */
		template <typename ShaderInputRange>
			explicit Program(
				ShaderInputRange shaders,
				ENABLE_IF(util::is_range<ShaderInputRange>::value));

		/**
		 * Creates a program proxy from a range of shaders.
		 */
		template <typename ShaderInputIterator>
			Program(
				ShaderInputIterator firstShader,
				ShaderInputIterator lastShader,
				ENABLE_IF(util::is_iterator<ShaderInputIterator>::value));

		private:
		/**
		 * Performs some additional initialization for the constructors.
		 */
		void Init();

		/*--------------------------+
		| BasicProxy implementation |
		+--------------------------*/

		pointer DoLock() const override;

		/*-------------+
		| data members |
		+-------------*/

		std::vector<Proxy<res::opengl::Shader>> shaders;
	};
}}}

#	include "ProgramProxy.tpp"
#endif
