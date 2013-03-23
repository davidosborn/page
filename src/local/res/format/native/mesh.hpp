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

#ifndef    page_local_res_format_native_mesh_hpp
#   define page_local_res_format_native_mesh_hpp

#	include <cstdint> // uint32_t

namespace page { namespace res { namespace format
{
	namespace native { namespace mesh
	{
		const char sig[] = {'P', 'A', 'G', 'E', 'm', 'e', 's', 'h'};

#	pragma pack(push, 1)
		struct Header
		{
			std::uint32_t faces;
			std::uint32_t vertices;
			std::uint32_t influences;
			std::uint32_t bones;
		};
		struct Face
		{
			std::uint32_t vertices[3];
		};
		struct Vertex
		{
			float co[3];
			float no[3];
			float uv[2];
			std::uint32_t influenceBase;
			std::uint32_t influences;
		};
		struct Influence
		{
			std::uint32_t bone;
			float weight;
		};
		struct Bone
		{
			std::uint32_t nameSize;
		};
#	pragma pack(pop)

		const char headerFormat[] = "dddd";
		const char faceFormat[] = "ddd";
		const char vertexFormat[] = "dddddddddd";
		const char influenceFormat[] = "dd";
		const char boneFormat[] = "d";
	}}

	using namespace native::mesh;
}}}

#endif
