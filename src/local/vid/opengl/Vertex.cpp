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
