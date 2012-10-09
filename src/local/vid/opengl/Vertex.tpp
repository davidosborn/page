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
					math::Vector<3> co, no;
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
