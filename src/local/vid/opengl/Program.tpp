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

#include <cassert>
#include <iostream> // clog
#include <vector>
#include "../../cfg/vars.hpp"
#include "../../err/Exception.hpp"
#include "../../log/Indenter.hpp"
#include "../../res/type/opengl/Shader.hpp" // Shader::{source,type}
#include "../../util/iterator/indirect_iterator.hpp"
#include "../../util/shared_handle.hpp"
#include "ext.hpp" // ARB_shader_objects

namespace page
{
	namespace vid
	{
		namespace opengl
		{
			// construct
			template <typename ShaderInputIterator>
				Program::Program(ShaderInputIterator first, ShaderInputIterator last)
			{
				Init(first, last);
			}

			// initialization
			template <typename ShaderInputIterator>
				void Program::Init(ShaderInputIterator first, ShaderInputIterator last)
			{
				assert(haveArbShaderObjects);
				typedef std::vector<util::shared_handle<GLhandleARB>> Shaders;
				Shaders shaders;
				for (ShaderInputIterator shader(first); shader != last; ++shader)
					shaders.push_back(util::make_shared_handle(
						MakeShader(shader->type, shader->source), glDeleteObjectARB));
				handle.reset(MakeProgram(
					util::make_indirect_iterator(shaders.begin()),
					util::make_indirect_iterator(shaders.end())), glDeleteObjectARB);
				InitUniforms();
			}

			// factory functions
			template <typename HandleInputIterator>
				GLhandleARB Program::MakeProgram(HandleInputIterator first, HandleInputIterator last)
			{
				GLhandleARB program = glCreateProgramObjectARB();
				if (!program)
					THROW((err::Exception<err::VidModuleTag, err::OpenglPlatformTag>("failed to create program object")))
				for (HandleInputIterator shader(first); shader != last; ++shader)
					glAttachObjectARB(program, *shader);
				glLinkProgramARB(program);
				GLint status;
				glGetObjectParameterivARB(program, GL_OBJECT_LINK_STATUS_ARB, &status);
				if (!status)
				{
					if (CVAR(logVerbose))
					{
						std::clog << "link error" << std::endl;
						log::Indenter indenter;
						std::clog << GetInfoLog(program) << std::endl;
					}
					glDeleteObjectARB(program);
					THROW((err::Exception<err::VidModuleTag, err::OpenglPlatformTag>("failed to link program object")))
				}
				return program;
			}
		}
	}
}
