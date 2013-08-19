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
