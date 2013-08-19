#include <cassert>
#include <iostream> // clog
#include <vector>

#include "../../cfg/vars.hpp"
#include "../../err/Exception.hpp"
#include "../../log/Indenter.hpp"
#include "../../res/type/opengl/Shader.hpp" // Shader::{source,type}
#include "../../util/iterator/indirect_iterator.hpp"
#include "../../util/raii/shared_handle.hpp"
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
					if (*CVAR(logVerbose))
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
