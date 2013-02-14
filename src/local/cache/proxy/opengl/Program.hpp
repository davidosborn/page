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

#	include <type_traits> // enable_if
#	include <vector>
#	include "../../../util/copy_ptr.hpp"
#	include "../../../util/type_traits/iterator.hpp" // is_iterator_over
#	include "../../Proxy.hpp"

namespace page
{
	namespace res {namespace opengl { class Shader; }}
	namespace vid {namespace opengl { class Program; }}

	namespace cache
	{
		namespace opengl
		{
			struct Program : Proxy<vid::opengl::Program>
			{
				using Proxy<vid::opengl::Program>::Instance;

				// construct
				explicit Program(const Proxy<res::opengl::Shader> &);
				Program(const Proxy<res::opengl::Shader> &, const Proxy<res::opengl::Shader> &);
				Program(const Proxy<res::opengl::Shader> &, const Proxy<res::opengl::Shader> &, const Proxy<res::opengl::Shader> &);
				Program(const Proxy<res::opengl::Shader> &, const Proxy<res::opengl::Shader> &, const Proxy<res::opengl::Shader> &, const Proxy<res::opengl::Shader> &);
				Program(const Proxy<res::opengl::Shader> &, const Proxy<res::opengl::Shader> &, const Proxy<res::opengl::Shader> &, const Proxy<res::opengl::Shader> &, const Proxy<res::opengl::Shader> &);
				Program(const Proxy<res::opengl::Shader> &, const Proxy<res::opengl::Shader> &, const Proxy<res::opengl::Shader> &, const Proxy<res::opengl::Shader> &, const Proxy<res::opengl::Shader> &, const Proxy<res::opengl::Shader> &);
				Program(const Proxy<res::opengl::Shader> &, const Proxy<res::opengl::Shader> &, const Proxy<res::opengl::Shader> &, const Proxy<res::opengl::Shader> &, const Proxy<res::opengl::Shader> &, const Proxy<res::opengl::Shader> &, const Proxy<res::opengl::Shader> &);
				Program(const Proxy<res::opengl::Shader> &, const Proxy<res::opengl::Shader> &, const Proxy<res::opengl::Shader> &, const Proxy<res::opengl::Shader> &, const Proxy<res::opengl::Shader> &, const Proxy<res::opengl::Shader> &, const Proxy<res::opengl::Shader> &, const Proxy<res::opengl::Shader> &);
				Program(const Proxy<res::opengl::Shader> &, const Proxy<res::opengl::Shader> &, const Proxy<res::opengl::Shader> &, const Proxy<res::opengl::Shader> &, const Proxy<res::opengl::Shader> &, const Proxy<res::opengl::Shader> &, const Proxy<res::opengl::Shader> &, const Proxy<res::opengl::Shader> &, const Proxy<res::opengl::Shader> &);
				Program(const Proxy<res::opengl::Shader> &, const Proxy<res::opengl::Shader> &, const Proxy<res::opengl::Shader> &, const Proxy<res::opengl::Shader> &, const Proxy<res::opengl::Shader> &, const Proxy<res::opengl::Shader> &, const Proxy<res::opengl::Shader> &, const Proxy<res::opengl::Shader> &, const Proxy<res::opengl::Shader> &, const Proxy<res::opengl::Shader> &);
				template <typename InputIterator> Program(InputIterator first, InputIterator last, typename std::enable_if<util::is_iterator_over<InputIterator, Proxy<res::opengl::Shader>>::value>::type * = 0);

				// clone
				Program *Clone() const;

				// attributes
				std::string GetType() const;
				std::string GetSource() const;

				// dependency satisfaction
				operator bool() const;

				private:
				// initialization
				void PostInit();

				// instantiation
				Instance Make() const;

				typedef std::vector<util::copy_ptr<Proxy<res::opengl::Shader>>> Shaders;
				Shaders shaders;
			};
		}
	}
}

#	include "Program.tpp"
#endif
