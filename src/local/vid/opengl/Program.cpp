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

#include <algorithm> // find, max, sort
#include <array>
#include <cassert>
#include <functional> // binary_function
#include <iomanip> // setw
#include <iostream> // c{log,out}
#include <string>
#include <vector>

#include <boost/io/ios_state.hpp> // ios_all_saver

#include "../../cfg/vars.hpp"
#include "../../err/Exception.hpp"
#include "../../log/Indenter.hpp"
#include "../../util/opengl/shader.hpp" // GetUniformTypeName
#include "../../util/opengl/string.hpp" // Native, String
#include "../../util/string.hpp" // Trim
#include "ext.hpp" // ARB_{{fragment,vertex}shader,shader_objects}
#include "Program.hpp"

namespace page
{
	namespace vid
	{
		namespace opengl
		{
			namespace
			{
				// uniform printing
				struct TableRow
				{
					std::string name, type;
					GLint location;
				};
				typedef std::vector<TableRow> Table;

				struct CompareTableRow :
					std::binary_function<TableRow, TableRow, bool>
				{
					bool operator ()(const TableRow &a, const TableRow &b) const
					{
						return
							a.location == b.location ?
							a.name     < b.name :
							a.location < b.location;
					}
				};
			}

			// construct
			Program::Program(const res::opengl::Shader &s1)
			{
				std::array<const res::opengl::Shader *, 1> shaders =
				{
					&s1
				};
				Init(
					util::make_indirect_iterator(shaders.begin()),
					util::make_indirect_iterator(shaders.end()));
			}
			Program::Program(const res::opengl::Shader &s1, const res::opengl::Shader &s2)
			{
				std::array<const res::opengl::Shader *, 2> shaders =
				{
					&s1, &s2
				};
				Init(
					util::make_indirect_iterator(shaders.begin()),
					util::make_indirect_iterator(shaders.end()));
			}
			Program::Program(const res::opengl::Shader &s1, const res::opengl::Shader &s2, const res::opengl::Shader &s3)
			{
				std::array<const res::opengl::Shader *, 3> shaders =
				{
					&s1, &s2, &s3
				};
				Init(
					util::make_indirect_iterator(shaders.begin()),
					util::make_indirect_iterator(shaders.end()));
			}
			Program::Program(const res::opengl::Shader &s1, const res::opengl::Shader &s2, const res::opengl::Shader &s3, const res::opengl::Shader &s4)
			{
				std::array<const res::opengl::Shader *, 4> shaders =
				{
					&s1, &s2, &s3, &s4
				};
				Init(
					util::make_indirect_iterator(shaders.begin()),
					util::make_indirect_iterator(shaders.end()));
			}
			Program::Program(const res::opengl::Shader &s1, const res::opengl::Shader &s2, const res::opengl::Shader &s3, const res::opengl::Shader &s4, const res::opengl::Shader &s5)
			{
				std::array<const res::opengl::Shader *, 5> shaders =
				{
					&s1, &s2, &s3, &s4, &s5
				};
				Init(
					util::make_indirect_iterator(shaders.begin()),
					util::make_indirect_iterator(shaders.end()));
			}
			Program::Program(const res::opengl::Shader &s1, const res::opengl::Shader &s2, const res::opengl::Shader &s3, const res::opengl::Shader &s4, const res::opengl::Shader &s5, const res::opengl::Shader &s6)
			{
				std::array<const res::opengl::Shader *, 6> shaders =
				{
					&s1, &s2, &s3, &s4, &s5, &s6
				};
				Init(
					util::make_indirect_iterator(shaders.begin()),
					util::make_indirect_iterator(shaders.end()));
			}
			Program::Program(const res::opengl::Shader &s1, const res::opengl::Shader &s2, const res::opengl::Shader &s3, const res::opengl::Shader &s4, const res::opengl::Shader &s5, const res::opengl::Shader &s6, const res::opengl::Shader &s7)
			{
				std::array<const res::opengl::Shader *, 7> shaders =
				{
					&s1, &s2, &s3, &s4, &s5, &s6, &s7
				};
				Init(
					util::make_indirect_iterator(shaders.begin()),
					util::make_indirect_iterator(shaders.end()));
			}
			Program::Program(const res::opengl::Shader &s1, const res::opengl::Shader &s2, const res::opengl::Shader &s3, const res::opengl::Shader &s4, const res::opengl::Shader &s5, const res::opengl::Shader &s6, const res::opengl::Shader &s7, const res::opengl::Shader &s8)
			{
				std::array<const res::opengl::Shader *, 8> shaders =
				{
					&s1, &s2, &s3, &s4, &s5, &s6, &s7, &s8
				};
				Init(
					util::make_indirect_iterator(shaders.begin()),
					util::make_indirect_iterator(shaders.end()));
			}
			Program::Program(const res::opengl::Shader &s1, const res::opengl::Shader &s2, const res::opengl::Shader &s3, const res::opengl::Shader &s4, const res::opengl::Shader &s5, const res::opengl::Shader &s6, const res::opengl::Shader &s7, const res::opengl::Shader &s8, const res::opengl::Shader &s9)
			{
				std::array<const res::opengl::Shader *, 9> shaders =
				{
					&s1, &s2, &s3, &s4, &s5, &s6, &s7, &s8, &s9
				};
				Init(
					util::make_indirect_iterator(shaders.begin()),
					util::make_indirect_iterator(shaders.end()));
			}
			Program::Program(const res::opengl::Shader &s1, const res::opengl::Shader &s2, const res::opengl::Shader &s3, const res::opengl::Shader &s4, const res::opengl::Shader &s5, const res::opengl::Shader &s6, const res::opengl::Shader &s7, const res::opengl::Shader &s8, const res::opengl::Shader &s9, const res::opengl::Shader &s10)
			{
				std::array<const res::opengl::Shader *, 10> shaders =
				{
					&s1, &s2, &s3, &s4, &s5, &s6, &s7, &s8, &s9, &s10
				};
				Init(
					util::make_indirect_iterator(shaders.begin()),
					util::make_indirect_iterator(shaders.end()));
			}

			// uniform access
			const Program::Uniform &Program::GetUniform(const std::string &name) const
			{
				const Uniform *uniform = FindUniform(name);
				if (!uniform)
					THROW((err::Exception<err::VidModuleTag, err::OpenglPlatformTag>(name + " is not an active uniform")))
				return *uniform;
			}
			const Program::Uniform *Program::FindUniform(const std::string &name) const
			{
				Uniforms::const_iterator iter(uniforms.find(name));
				return iter != uniforms.end() ? &iter->second : 0;
			}

			// handle access
			GLhandleARB Program::GetHandle() const
			{
				return *handle;
			}

			// initialization
			void Program::InitUniforms()
			{
				GLint activeUniforms;
				glGetObjectParameterivARB(*handle, GL_OBJECT_ACTIVE_UNIFORMS_ARB, &activeUniforms);
				if (glGetError())
					THROW((err::Exception<err::VidModuleTag, err::OpenglPlatformTag>("failed to get number of uniforms")))
				for (unsigned i = 0; i < activeUniforms; ++i)
				{
					std::vector<GLcharARB> nameBuffer(32);
					GLint size;
					GLenum type;
					for (;;)
					{
						GLsizei length;
						glGetActiveUniformARB(*handle, i, nameBuffer.size(), &length, &size, &type, &*nameBuffer.begin());
						if (glGetError())
							THROW((err::Exception<err::VidModuleTag, err::OpenglPlatformTag>("failed to get uniform information")))
						if (length < nameBuffer.size() - 1)
						{
							nameBuffer.resize(length);
							break;
						}
						nameBuffer.resize(nameBuffer.size() * 2);
					}
					std::string name(nameBuffer.begin(), std::find(nameBuffer.begin(), nameBuffer.end(), '['));
					GLint location = glGetUniformLocationARB(*handle, name.c_str());
					if (glGetError())
						THROW((err::Exception<err::VidModuleTag, err::OpenglPlatformTag>("failed to get uniform location")))
					Uniform uniform = {name, size, type, location};
					uniforms.insert(std::make_pair(name, uniform));
				}
				if (*CVAR(logVerbose))
				{
					std::cout << "enumerating uniforms" << std::endl;
					log::Indenter indenter;

					// build table of uniform data
					Table table;
					table.reserve(uniforms.size());
					for (Uniforms::const_iterator iter(uniforms.begin()); iter != uniforms.end(); ++iter)
					{
						const Uniform &uniform(iter->second);
						TableRow row =
						{
							uniform.name,
							util::opengl::GetUniformTypeName(uniform.type),
							uniform.location
						};
						if (uniform.size > 1)
							row.name += std::string("[") + util::lexical_cast<std::string>(uniform.size) + ']';
						table.push_back(row);
					}

					// sort table
					std::sort(table.begin(), table.end(), CompareTableRow());

					// calculate alignment
					std::string::size_type width = 0;
					for (Table::const_iterator row(table.begin()); row != table.end(); ++row)
						width = std::max(row->type.size(), width);

					// set flags for trailing alignment
					boost::io::ios_all_saver iosFormatSaver(std::cout);
					std::cout.setf(std::ios_base::left, std::ios_base::adjustfield);
					std::cout.fill(' ');

					// print table
					for (Table::const_iterator row(table.begin()); row != table.end(); ++row)
						std::cout << std::setw(width + 1) << row->type << row->name << std::endl;
				}
			}

			// factory functions
			GLhandleARB Program::MakeShader(GLenum type, const std::string &source)
			{
				assert(type != GL_VERTEX_SHADER_ARB || haveArbVertexShader);
				assert(type != GL_FRAGMENT_SHADER_ARB || haveArbFragmentShader);
				GLhandleARB shader = glCreateShaderObjectARB(type);
				if (!shader)
					THROW((err::Exception<err::VidModuleTag, err::OpenglPlatformTag>("failed to create shader object")))
				util::opengl::String ns(util::opengl::Native(source));
				const GLcharARB *src = ns.c_str();
				glShaderSourceARB(shader, 1, &src, 0);
				glCompileShaderARB(shader);
				GLint status;
				glGetObjectParameterivARB(shader, GL_OBJECT_COMPILE_STATUS_ARB, &status);
				if (!status)
				{
					if (*CVAR(logVerbose))
					{
						std::clog << "compile error" << std::endl;
						log::Indenter indenter;
						std::clog << GetInfoLog(shader) << std::endl;
					}
					glDeleteObjectARB(shader);
					THROW((err::Exception<err::VidModuleTag, err::OpenglPlatformTag>("failed to compile shader")))
				}
				return shader;
			}

			// compile/link information
			std::string Program::GetInfoLog(GLhandleARB handle)
			{
				GLint bufferSize = 0;
				glGetObjectParameterivARB(handle, GL_OBJECT_INFO_LOG_LENGTH_ARB, &bufferSize);
				std::vector<GLcharARB> buffer(bufferSize);
				glGetInfoLogARB(handle, buffer.size(), 0, &*buffer.begin());
				return util::Trim(std::string(buffer.begin(), buffer.end()));
			}

			// binding
			void Bind(const Program &program)
			{
				glUseProgramObjectARB(program.GetHandle());
			}
		}
	}
}
