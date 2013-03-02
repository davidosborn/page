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
#include <functional> // bind, mem_fn

#include "../../../util/functional/pointer.hpp" // dereference
#include "../../../util/io/serialize.hpp"
#include "../../../util/iterator/call_iterator.hpp"
#include "../../../util/iterator/indirect_iterator.hpp"
#include "../../../vid/opengl/Program.hpp"
#include "Program.hpp"

namespace page
{
	namespace cache
	{
		namespace opengl
		{
			/*------+
			| clone |
			+------*/

			Program *Program::Clone() const
			{
				return new Program(*this);
			}

			/*----------+
			| observers |
			+----------*/

			std::string Program::GetType() const
			{
				return "program";
			}

			std::string Program::GetSource() const
			{
				return util::Serialize<char>(
					util::make_call_iterator(util::make_indirect_iterator(shaders.begin()),
						std::mem_fn(&Proxy<res::opengl::Shader>::GetSource)),
					util::make_call_iterator(util::make_indirect_iterator(shaders.end()),
						std::mem_fn(&Proxy<res::opengl::Shader>::GetSource)), ',');
			}

			Program::operator bool() const
			{
				return std::find(
					util::make_indirect_iterator(shaders.begin()),
					util::make_indirect_iterator(shaders.end()),
					false).base() == shaders.end();
			}

			/*--------------+
			| instantiation |
			+--------------*/

			Program::Instance Program::Make() const
			{
				return Instance(new vid::opengl::Program(
					util::make_indirect_iterator(util::make_indirect_iterator(shaders.begin())),
					util::make_indirect_iterator(util::make_indirect_iterator(shaders.end()))));
			}

			/*---------------+
			| initialization |
			+---------------*/

			void Program::PostInit()
			{
				// ensure pointers are valid
				assert(std::find(shaders.begin(), shaders.end(), nullptr) == shaders.end());

				// sort shaders by source
				using namespace std::placeholders;
				sort(shaders.begin(), shaders.end(),
					bind(Proxy<res::opengl::Shader>::CompareSource(),
						bind(util::dereference<util::copy_ptr<Proxy<res::opengl::Shader>>>(), _1),
						bind(util::dereference<util::copy_ptr<Proxy<res::opengl::Shader>>>(), _2)));
			}
		}
	}
}
