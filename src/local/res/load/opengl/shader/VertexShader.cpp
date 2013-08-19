#include <cassert>
#include <memory> // {shared,unique}_ptr

#include <GL/gl.h> // glext.h
#include <GL/glext.h> // GL_VERTEX_SHADER_ARB

#include "../../../pipe/Pipe.hpp" // Pipe::Open
#include "../../../pipe/Stream.hpp" // Stream::GetText
#include "../../../type/opengl/Shader.hpp"
#include "../../LoaderRegistry.hpp" // REGISTER_LOADER

namespace page
{
	namespace res
	{
		namespace opengl
		{
			Shader *LoadVertexShader(const std::shared_ptr<const Pipe> &pipe)
			{
				assert(pipe);
				const std::unique_ptr<Stream> stream(pipe->Open());
				const std::unique_ptr<Shader> shader(new Shader);
				shader->type = GL_VERTEX_SHADER_ARB;
				shader->source = stream->GetText();
				return shader.release();
			}

			LoadFunction GetVertexShaderLoader(const Pipe &pipe)
			{
				return LoadVertexShader;
			}

			REGISTER_LOADER(Shader, GetVertexShaderLoader,
				"vs", "", "", false)
		}
	}
}
