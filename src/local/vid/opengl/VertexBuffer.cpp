/**
 * @copyright
 *
 * Copyright (c) 2006-2014 David Osborn
 *
 * Permission is granted to use and redistribute this software in source and
 * binary form, with or without modification, subject to the following
 * conditions:
 *
 * 1. Redistributions in source form must retain the above copyright notice,
 *    this list of conditions, and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions, and the following disclaimer in the same place
 *    and form as other copyright, license, and disclaimer information.
 *
 * 3. Redistributions in binary form must also include an acknowledgement in the
 *    same place and form as other acknowledgements (such as the credits),
 *    similar in substance to the following:
 *
 *       Portions of this software are based on the work of David Osborn.
 *
 * This software is provided "as is", without any express or implied warranty.
 * In no event will the authors be liable for any damages arising out of the use
 * of this software.
 */

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
