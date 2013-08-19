#ifndef    page_local_cache_proxy_opengl_Program_hpp
#   define page_local_cache_proxy_opengl_Program_hpp

#	include <vector>

#	include "../../../util/type_traits/iterator.hpp" // is_iterator
#	include "../../../util/type_traits/range.hpp" // is_range
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
		std::vector<Proxy<res::opengl::Shader>> shaders;
	};
}}}

#	include "Program.tpp"
#endif
