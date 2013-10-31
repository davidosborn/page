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
