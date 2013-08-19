#include <cassert>
#include <cstddef> // offsetof

#include "ext.hpp" // ARB_{multitexture,vertex_shader}
#include "Vertex.hpp" // Vertex{,Format}

namespace page
{
	namespace vid
	{
		namespace opengl
		{
			// vertex format construct
			VertexFormat::VertexFormat() : normal(false), tangent(false) {}

			void PrepVertices(const Vertex *vertices, const VertexFormat &format)
			{
				// initialize texture coordinate pointers
				if (haveArbMultitexture)
				{
					unsigned texIndex = 0;
					for (VertexFormat::UvIndices::const_iterator uvIndex(format.uvIndices.begin()); uvIndex != format.uvIndices.end(); ++uvIndex, ++texIndex)
					{
						assert(*uvIndex < sizeof vertices->uv / sizeof *vertices->uv);
						glClientActiveTextureARB(GL_TEXTURE0_ARB + texIndex);
						glTexCoordPointer(2, GL_FLOAT, sizeof *vertices, vertices->uv[*uvIndex]);
						glEnableClientState(GL_TEXTURE_COORD_ARRAY);
					}
				}
				else if (!format.uvIndices.empty())
				{
					assert(format.uvIndices.size() == 1);
					assert(format.uvIndices.front() < sizeof vertices->uv / sizeof *vertices->uv);
					glTexCoordPointer(2, GL_FLOAT, sizeof *vertices, vertices->uv[format.uvIndices.front()]);
					glEnableClientState(GL_TEXTURE_COORD_ARRAY);
				}
				// initialize normal pointer
				if (format.normal)
				{
					// FIXME: this is causing a segfault in glDrawElements when
					// using vertex arrays
					glNormalPointer(GL_FLOAT, sizeof *vertices, vertices->no);
					glEnableClientState(GL_NORMAL_ARRAY);
				}
				// initialize tangent pointer
				if (format.tangent)
				{
					assert(haveArbVertexShader);
					glVertexAttribPointerARB(0, 3, GL_FLOAT, GL_TRUE, sizeof *vertices, vertices->tan);
					glEnableVertexAttribArrayARB(0);
				}
				// initialize vertex pointer
				glVertexPointer(3, GL_FLOAT, sizeof *vertices, vertices->co);
				glEnableClientState(GL_VERTEX_ARRAY);
			}
		}
	}
}
