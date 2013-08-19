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
