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

// program object wrapper

#ifndef    page_local_vid_opengl_Program_hpp
#   define page_local_vid_opengl_Program_hpp

#	include <string>
#	include <unordered_map>

#	include <GL/gl.h> // GL{enum,int}, glext.h
#	include <GL/glext.h> // GLhandleARB

#	include "../../util/NonCopyable.hpp"
#	include "../../util/raii/unique_handle.hpp"

namespace page
{
	namespace res {namespace opengl { class Shader; }}

	namespace vid
	{
		namespace opengl
		{
			struct Program : util::NonCopyable
			{
				// construct
				explicit Program(const res::opengl::Shader &);
				Program(const res::opengl::Shader &, const res::opengl::Shader &);
				Program(const res::opengl::Shader &, const res::opengl::Shader &, const res::opengl::Shader &);
				Program(const res::opengl::Shader &, const res::opengl::Shader &, const res::opengl::Shader &, const res::opengl::Shader &);
				Program(const res::opengl::Shader &, const res::opengl::Shader &, const res::opengl::Shader &, const res::opengl::Shader &, const res::opengl::Shader &);
				Program(const res::opengl::Shader &, const res::opengl::Shader &, const res::opengl::Shader &, const res::opengl::Shader &, const res::opengl::Shader &, const res::opengl::Shader &);
				Program(const res::opengl::Shader &, const res::opengl::Shader &, const res::opengl::Shader &, const res::opengl::Shader &, const res::opengl::Shader &, const res::opengl::Shader &, const res::opengl::Shader &);
				Program(const res::opengl::Shader &, const res::opengl::Shader &, const res::opengl::Shader &, const res::opengl::Shader &, const res::opengl::Shader &, const res::opengl::Shader &, const res::opengl::Shader &, const res::opengl::Shader &);
				Program(const res::opengl::Shader &, const res::opengl::Shader &, const res::opengl::Shader &, const res::opengl::Shader &, const res::opengl::Shader &, const res::opengl::Shader &, const res::opengl::Shader &, const res::opengl::Shader &, const res::opengl::Shader &);
				Program(const res::opengl::Shader &, const res::opengl::Shader &, const res::opengl::Shader &, const res::opengl::Shader &, const res::opengl::Shader &, const res::opengl::Shader &, const res::opengl::Shader &, const res::opengl::Shader &, const res::opengl::Shader &, const res::opengl::Shader &);
				template <typename ShaderInputIterator>
					Program(
						ShaderInputIterator first,
						ShaderInputIterator last);

				// uniform access
				struct Uniform
				{
					std::string name;
					unsigned size;
					GLenum type;
					GLint location;
				};
				const Uniform &GetUniform(const std::string &name) const;
				const Uniform *FindUniform(const std::string &name) const;

				// handle access
				GLhandleARB GetHandle() const;

				private:
				// initialization
				template <typename ShaderInputIterator>
					void Init(
						ShaderInputIterator first,
						ShaderInputIterator last);
				void InitUniforms();

				// factory functions
				template <typename HandleInputIterator>
					static GLhandleARB MakeProgram(
						HandleInputIterator first,
						HandleInputIterator last);
				static GLhandleARB MakeShader(GLenum type, const std::string &source);

				// compile/link information
				static std::string GetInfoLog(GLhandleARB);

				util::unique_handle<GLhandleARB> handle;
				typedef std::unordered_map<std::string, Uniform> Uniforms;
				Uniforms uniforms;
			};

			// binding
			void Bind(const Program &);
		}
	}
}

#	include "Program.tpp"
#endif
