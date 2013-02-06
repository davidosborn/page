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

#include <iterator> // back_inserter
#include <GL/gl.h>
#include "../../res/type/Mesh.hpp"
#include "ClientAttribGuard.hpp"
#include "VertexArray.hpp"

namespace page
{
	namespace vid
	{
		namespace opengl
		{
			VertexArray::VertexArray(const res::Mesh &mesh)
			{
				indices.reserve(mesh.faces.size() * 3);
				InitIndices(std::back_inserter(indices), mesh);
				vertices.reserve(mesh.vertices.size());
				InitVertices(std::back_inserter(vertices), mesh);
			}

			void VertexArray::Update(const phys::Skin &skin)
			{
				UpdateVertices(vertices.begin(), skin);
			}
			void VertexArray::Draw(const VertexFormat &format) const
			{
				ClientAttribGuard clientAttribGuard(GL_CLIENT_VERTEX_ARRAY_BIT);
				PrepVertices(&*vertices.begin(), format);
				DrawIndices(&*indices.begin(), indices.size());
			}
		}
	}
}
