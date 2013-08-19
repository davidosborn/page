#ifndef    page_local_res_type_Mesh_hpp
#   define page_local_res_type_Mesh_hpp

#	include <array>
#	include <string>
#	include <vector>

#	include "../../math/Vector.hpp"

namespace page
{
	namespace res
	{
		struct Mesh
		{
			// construct/assign
			Mesh();
			Mesh(const Mesh &);
			Mesh &operator =(const Mesh &);

			typedef std::vector<std::string> Bones;
			Bones bones;
			struct Vertex
			{
				math::Vec3 co, no;
				math::Vec2 uv[3];
				struct Influence
				{
					Bones::const_iterator bone;
					float weight;
				};
				typedef std::vector<Influence> Influences;
				Influences influences;
			};
			typedef std::vector<Vertex> Vertices;
			Vertices vertices;
			struct Face
			{
				typedef std::array<Vertices::const_iterator, 3> VertexIndices;
				VertexIndices vertices;
			};
			typedef std::vector<Face> Faces;
			Faces faces;
		};

		void GenNormals(Mesh &);
		void SortInfluences(Mesh &);
	}
}

#endif
