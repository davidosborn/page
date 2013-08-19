#include "../../phys/Skin.hpp"
#include "../../res/type/Mesh.hpp"

namespace page
{
	namespace vid
	{
		namespace opengl
		{
			template <typename OutputIterator> void InitVertices(OutputIterator iter, const res::Mesh &mesh)
			{
				for (res::Mesh::Vertices::const_iterator meshVertex(mesh.vertices.begin()); meshVertex != mesh.vertices.end(); ++meshVertex)
				{
					Vertex vertex =
					{
						meshVertex->uv[0].x, meshVertex->uv[0].y,
						meshVertex->uv[1].x, meshVertex->uv[1].y,
						meshVertex->uv[2].x, meshVertex->uv[2].y,
						meshVertex->no.x, meshVertex->no.y, meshVertex->no.z,
						0, 0, 0, // tangent
						meshVertex->co.x, meshVertex->co.y, meshVertex->co.z
					};
					*iter++ = vertex;
				}
				// FIXME: calculate tangents if necessary
			}
			template <typename Iterator> void UpdateVertices(Iterator iter, const phys::Skin &skin)
			{
				for (phys::Skin::Vertices::const_iterator skinVertex(skin.vertices.begin()); skinVertex != skin.vertices.end(); ++skinVertex)
				{
					math::Vec3 co, no;
					Update(*skinVertex, co, no);
					iter->no[0] = no.x;
					iter->no[1] = no.y;
					iter->no[2] = no.z;
					iter->co[0] = co.x;
					iter->co[1] = co.y;
					iter->co[2] = co.z;
					++iter;
				}
				// FIXME: recalculate tangents if necessary
			}
		}
	}
}
