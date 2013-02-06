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

#include <algorithm> // find, sort
#include <functional> // bind, mem_fun_ref
#include "../../../util/functional.hpp" // dereference
#include "../../../util/iterator/call_iterator.hpp"
#include "../../../util/iterator/indirect_iterator.hpp"
#include "../../../util/serialize/serialize_string.hpp" // Serialize
#include "../../../vid/opengl/Program.hpp"
#include "Program.hpp"

namespace page
{
	namespace cache
	{
		namespace opengl
		{
			// construct
			Program::Program(const Proxy<res::opengl::Shader> &shader) :
				shaders(1, shader.Copy()) {}
			Program::Program(const Proxy<res::opengl::Shader> &shader1, const Proxy<res::opengl::Shader> &shader2)
			{
				shaders.reserve(2);
				shaders.push_back(shader1.Copy());
				shaders.push_back(shader2.Copy());
				PostInit();
			}
			Program::Program(const Proxy<res::opengl::Shader> &shader1, const Proxy<res::opengl::Shader> &shader2, const Proxy<res::opengl::Shader> &shader3)
			{
				shaders.reserve(3);
				shaders.push_back(shader1.Copy());
				shaders.push_back(shader2.Copy());
				shaders.push_back(shader3.Copy());
				PostInit();
			}
			Program::Program(const Proxy<res::opengl::Shader> &shader1, const Proxy<res::opengl::Shader> &shader2, const Proxy<res::opengl::Shader> &shader3, const Proxy<res::opengl::Shader> &shader4)
			{
				shaders.reserve(4);
				shaders.push_back(shader1.Copy());
				shaders.push_back(shader2.Copy());
				shaders.push_back(shader3.Copy());
				shaders.push_back(shader4.Copy());
				PostInit();
			}
			Program::Program(const Proxy<res::opengl::Shader> &shader1, const Proxy<res::opengl::Shader> &shader2, const Proxy<res::opengl::Shader> &shader3, const Proxy<res::opengl::Shader> &shader4, const Proxy<res::opengl::Shader> &shader5)
			{
				shaders.reserve(5);
				shaders.push_back(shader1.Copy());
				shaders.push_back(shader2.Copy());
				shaders.push_back(shader3.Copy());
				shaders.push_back(shader4.Copy());
				shaders.push_back(shader5.Copy());
				PostInit();
			}
			Program::Program(const Proxy<res::opengl::Shader> &shader1, const Proxy<res::opengl::Shader> &shader2, const Proxy<res::opengl::Shader> &shader3, const Proxy<res::opengl::Shader> &shader4, const Proxy<res::opengl::Shader> &shader5, const Proxy<res::opengl::Shader> &shader6)
			{
				shaders.reserve(6);
				shaders.push_back(shader1.Copy());
				shaders.push_back(shader2.Copy());
				shaders.push_back(shader3.Copy());
				shaders.push_back(shader4.Copy());
				shaders.push_back(shader5.Copy());
				shaders.push_back(shader6.Copy());
				PostInit();
			}
			Program::Program(const Proxy<res::opengl::Shader> &shader1, const Proxy<res::opengl::Shader> &shader2, const Proxy<res::opengl::Shader> &shader3, const Proxy<res::opengl::Shader> &shader4, const Proxy<res::opengl::Shader> &shader5, const Proxy<res::opengl::Shader> &shader6, const Proxy<res::opengl::Shader> &shader7)
			{
				shaders.reserve(7);
				shaders.push_back(shader1.Copy());
				shaders.push_back(shader2.Copy());
				shaders.push_back(shader3.Copy());
				shaders.push_back(shader4.Copy());
				shaders.push_back(shader5.Copy());
				shaders.push_back(shader6.Copy());
				shaders.push_back(shader7.Copy());
				PostInit();
			}
			Program::Program(const Proxy<res::opengl::Shader> &shader1, const Proxy<res::opengl::Shader> &shader2, const Proxy<res::opengl::Shader> &shader3, const Proxy<res::opengl::Shader> &shader4, const Proxy<res::opengl::Shader> &shader5, const Proxy<res::opengl::Shader> &shader6, const Proxy<res::opengl::Shader> &shader7, const Proxy<res::opengl::Shader> &shader8)
			{
				shaders.reserve(8);
				shaders.push_back(shader1.Copy());
				shaders.push_back(shader2.Copy());
				shaders.push_back(shader3.Copy());
				shaders.push_back(shader4.Copy());
				shaders.push_back(shader5.Copy());
				shaders.push_back(shader6.Copy());
				shaders.push_back(shader7.Copy());
				shaders.push_back(shader8.Copy());
				PostInit();
			}
			Program::Program(const Proxy<res::opengl::Shader> &shader1, const Proxy<res::opengl::Shader> &shader2, const Proxy<res::opengl::Shader> &shader3, const Proxy<res::opengl::Shader> &shader4, const Proxy<res::opengl::Shader> &shader5, const Proxy<res::opengl::Shader> &shader6, const Proxy<res::opengl::Shader> &shader7, const Proxy<res::opengl::Shader> &shader8, const Proxy<res::opengl::Shader> &shader9)
			{
				shaders.reserve(9);
				shaders.push_back(shader1.Copy());
				shaders.push_back(shader2.Copy());
				shaders.push_back(shader3.Copy());
				shaders.push_back(shader4.Copy());
				shaders.push_back(shader5.Copy());
				shaders.push_back(shader6.Copy());
				shaders.push_back(shader7.Copy());
				shaders.push_back(shader8.Copy());
				shaders.push_back(shader9.Copy());
				PostInit();
			}
			Program::Program(const Proxy<res::opengl::Shader> &shader1, const Proxy<res::opengl::Shader> &shader2, const Proxy<res::opengl::Shader> &shader3, const Proxy<res::opengl::Shader> &shader4, const Proxy<res::opengl::Shader> &shader5, const Proxy<res::opengl::Shader> &shader6, const Proxy<res::opengl::Shader> &shader7, const Proxy<res::opengl::Shader> &shader8, const Proxy<res::opengl::Shader> &shader9, const Proxy<res::opengl::Shader> &shader10)
			{
				shaders.reserve(10);
				shaders.push_back(shader1.Copy());
				shaders.push_back(shader2.Copy());
				shaders.push_back(shader3.Copy());
				shaders.push_back(shader4.Copy());
				shaders.push_back(shader5.Copy());
				shaders.push_back(shader6.Copy());
				shaders.push_back(shader7.Copy());
				shaders.push_back(shader8.Copy());
				shaders.push_back(shader9.Copy());
				shaders.push_back(shader10.Copy());
				PostInit();
			}

			// clone
			Program *Program::Clone() const
			{
				return new Program(*this);
			}

			// attributes
			std::string Program::GetType() const
			{
				return "program";
			}
			std::string Program::GetSource() const
			{
				return util::Serialize<char>(
					util::make_call_iterator(util::make_indirect_iterator(shaders.begin()),
						std::mem_fun_ref(&Proxy<res::opengl::Shader>::GetSource)),
					util::make_call_iterator(util::make_indirect_iterator(shaders.end()),
						std::mem_fun_ref(&Proxy<res::opengl::Shader>::GetSource)), ',');
			}

			// dependency satisfaction
			Program::operator bool() const
			{
				return std::find(
					util::make_indirect_iterator(shaders.begin()),
					util::make_indirect_iterator(shaders.end()),
					false).base() == shaders.end();
			}

			// initialization
			void Program::PostInit()
			{
				// ensure pointers are valid
				assert(std::find(shaders.begin(), shaders.end(), nullptr) == shaders.end());
				// sort shaders by source
				using namespace std::placeholders;
				std::sort(shaders.begin(), shaders.end(),
					bind(CompareProxySource<res::opengl::Shader>(),
						bind(util::dereference<util::copy_ptr<Proxy<res::opengl::Shader>>>(), _1),
						bind(util::dereference<util::copy_ptr<Proxy<res::opengl::Shader>>>(), _2)));
			}

			// instantiation
			Program::Instance Program::Make() const
			{
				return Instance(new vid::opengl::Program(
					util::make_indirect_iterator(util::make_indirect_iterator(shaders.begin())),
					util::make_indirect_iterator(util::make_indirect_iterator(shaders.end()))));
			}
		}
	}
}
