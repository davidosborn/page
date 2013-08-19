// program object wrapper

#ifndef    page_local_vid_opengl_Program_hpp
#   define page_local_vid_opengl_Program_hpp

#	include <string>
#	include <unordered_map>

#	include <GL/gl.h> // GL{enum,int}, glext.h
#	include <GL/glext.h> // GLhandleARB

#	include "../../util/class/special_member_functions.hpp" // Uncopyable
#	include "../../util/raii/unique_handle.hpp"

namespace page
{
	namespace res { namespace opengl { class Shader; }}

	namespace vid
	{
		namespace opengl
		{
			struct Program : util::Uncopyable<Program>
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
