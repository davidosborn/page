#include "../res/type/Mesh.hpp"
#include "Skin.hpp"

namespace page
{
	namespace phys
	{
		// construct
		Skin::Skin(const res::Mesh &mesh, const attrib::Pose &pose)
		{
			// copy vertices
			vertices.reserve(mesh.vertices.size());
			for (res::Mesh::Vertices::const_iterator meshVertex(mesh.vertices.begin()); meshVertex != mesh.vertices.end(); ++meshVertex)
			{
				Vertex vertex =
				{
					meshVertex->co,
					meshVertex->no
				};
				// copy influences
				vertex.influences.reserve(meshVertex->influences.size());
				for (res::Mesh::Vertex::Influences::const_iterator meshInfluence(meshVertex->influences.begin()); meshInfluence != meshVertex->influences.end(); ++meshInfluence)
				{
					const attrib::Pose::Bone *bone(pose.GetBone(*meshInfluence->bone));
					if (bone)
					{
						Vertex::Influence influence =
						{
							bone,
							meshInfluence->weight
						};
						vertex.influences.push_back(influence);
					}
				}
				vertices.push_back(vertex);
			}
		}

		// transform vertex
		void Update(const Skin::Vertex &vertex, math::Vec3 &co, math::Vec3 &no)
		{
			co = no = 0;
			float weight = 1.f;
			for (Skin::Vertex::Influences::const_iterator influence(vertex.influences.begin()); influence != vertex.influences.end(); ++influence)
			{
				co += influence->bone->GetSkinMatrix() * vertex.co * influence->weight;
				no += influence->bone->GetNormSkinMatrix() * vertex.no * influence->weight;
				weight -= influence->weight;
			}
			if (weight > 0.f)
			{
				co += vertex.co * weight;
				no += vertex.no * weight;
			}
		}
	}
}
