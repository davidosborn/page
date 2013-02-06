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
		void Update(const Skin::Vertex &vertex, math::Vector<3> &co, math::Vector<3> &no)
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
