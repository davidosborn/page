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

#include "Mesh.hpp"
#include "register.hpp" // REGISTER_TYPE

namespace page
{
	namespace res
	{
		// construct/assign
		Mesh::Mesh() {}
		Mesh::Mesh(const Mesh &other) :
			bones(other.bones), vertices(other.vertices), faces(other.faces)
		{
			// remap bone iterators
			// FIXME: implement
			// remap vertex iterators
			// FIXME: implement
		}
		Mesh &Mesh::operator =(const Mesh &other)
		{
			bones = other.bones;
			vertices = other.vertices;
			faces = other.faces;
			// remap bone iterators
			// FIXME: implement
			// remap vertex iterators
			// FIXME: implement
		}

		void GenNormals(Mesh &mesh)
		{
			// FIXME: implement; do not generate normals if already existing
		}
		void SortInfluences(Mesh &mesh)
		{
			// FIXME: implement; do not sort influences if already sorted
		}

		void PostLoadMesh(Mesh &mesh)
		{
			GenNormals(mesh);
			SortInfluences(mesh);
		}

		REGISTER_TYPE(Mesh, "mesh", PostLoadMesh)
	}
}
