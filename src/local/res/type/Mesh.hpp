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
