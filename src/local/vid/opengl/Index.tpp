#include <iterator> // distance

#include "../../res/type/Mesh.hpp"

namespace page
{
	namespace vid
	{
		namespace opengl
		{
			template <typename OutputIterator> void InitIndices(OutputIterator iter, const res::Mesh &mesh)
			{
				for (res::Mesh::Faces::const_iterator meshFace(mesh.faces.begin()); meshFace != mesh.faces.end(); ++meshFace)
					for (res::Mesh::Face::VertexIndices::const_iterator meshVertex(meshFace->vertices.begin()); meshVertex != meshFace->vertices.end(); ++meshVertex)
						*iter++ = std::distance(mesh.vertices.begin(), *meshVertex);
			}
		}
	}
}
