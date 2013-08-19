#include <cassert>

#include "../../err/Exception.hpp"
#include "../../res/type/Mesh.hpp"
#include "ClientAttribGuard.hpp"
#include "ext.hpp" // ARB_vertex_buffer_object
#include "Index.hpp"
#include "VertexBuffer.hpp"

namespace page
{
	namespace vid
	{
		namespace opengl
		{
			VertexBuffer::VertexBuffer(const res::Mesh &mesh, GLenum usage) :
				numIndices(mesh.faces.size() * 3)
			{
				assert(haveArbVertexBufferObject);
				// initialize index buffer
				if (glGenBuffersARB(1, &indices), glGetError())
					THROW((err::Exception<err::VidModuleTag, err::OpenglPlatformTag>("failed to generate index buffer")))
				glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, indices);
				glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB, sizeof(Index) * numIndices, 0, GL_STATIC_DRAW_ARB);
				Index *mappedIndices = static_cast<Index *>(glMapBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, GL_WRITE_ONLY_ARB));
				if (glGetError())
				{
					glDeleteBuffersARB(1, &indices);
					THROW((err::Exception<err::VidModuleTag, err::OpenglPlatformTag>("failed to map index buffer")))
				}
				InitIndices(mappedIndices, mesh);
				glUnmapBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB);
				// initialize vertex buffer
				if (glGenBuffersARB(1, &vertices), glGetError())
				{
					glDeleteBuffersARB(1, &indices);
					THROW((err::Exception<err::VidModuleTag, err::OpenglPlatformTag>("failed to generate vertex buffer")))
				}
				glBindBufferARB(GL_ARRAY_BUFFER_ARB, vertices);
				glBufferDataARB(GL_ARRAY_BUFFER_ARB, sizeof(Vertex) * mesh.vertices.size(), 0, usage);
				Vertex *mappedVertices = static_cast<Vertex *>(glMapBufferARB(GL_ARRAY_BUFFER_ARB, GL_WRITE_ONLY_ARB));
				if (glGetError())
				{
					glDeleteBuffersARB(1, &vertices);
					glDeleteBuffersARB(1, &indices);
					THROW((err::Exception<err::VidModuleTag, err::OpenglPlatformTag>("failed to map vertex buffer")))
				}
				InitVertices(mappedVertices, mesh);
				glUnmapBufferARB(GL_ARRAY_BUFFER_ARB);
			}
			VertexBuffer::~VertexBuffer()
			{
				glDeleteBuffersARB(1, &vertices);
				glDeleteBuffersARB(1, &indices);
			}

			void VertexBuffer::Update(const phys::Skin &skin)
			{
				glBindBufferARB(GL_ARRAY_BUFFER_ARB, vertices);
				Vertex *mappedVertices = static_cast<Vertex *>(glMapBufferARB(GL_ARRAY_BUFFER_ARB, GL_WRITE_ONLY_ARB));
				if (!mappedVertices)
					THROW((err::Exception<err::VidModuleTag, err::OpenglPlatformTag>("failed to map vertex buffer")))
				UpdateVertices(mappedVertices, skin);
				glUnmapBufferARB(GL_ARRAY_BUFFER_ARB);
			}
			void VertexBuffer::Draw(const VertexFormat &format) const
			{
				ClientAttribGuard clientAttribGuard(GL_CLIENT_VERTEX_ARRAY_BIT);
				glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, indices);
				glBindBufferARB(GL_ARRAY_BUFFER_ARB, vertices);
				PrepVertices(0, format);
				DrawIndices(0, numIndices);
			}
		}
	}
}
