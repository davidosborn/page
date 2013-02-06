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

#ifndef    page_local_vid_opengl_Vertex_hpp
#   define page_local_vid_opengl_Vertex_hpp

#	include <vector>
#	include <GL/gl.h> // GLfloat

namespace page
{
	namespace phys { class Skin; }
	namespace res { class Mesh; }

	namespace vid
	{
		namespace opengl
		{
#	pragma pack(push, 1)
			struct Vertex
			{
				GLfloat
					uv[2][3], // texture coordinates
					no[3],    // normal
					tan[3],   // tangent
					co[3],    // position
					padding;
			};
#	pragma pack(pop)

			template <typename OutputIterator> void InitVertices(OutputIterator, const res::Mesh &);
			template <typename Iterator> void UpdateVertices(Iterator, const phys::Skin &);

			struct VertexFormat
			{
				// construct
				VertexFormat();

				typedef std::vector<unsigned> UvIndices;
				UvIndices uvIndices;
				bool normal, tangent;
			};
			void PrepVertices(const Vertex *, const VertexFormat & = VertexFormat());
		}
	}
}

#	include "Vertex.tpp"
#endif
