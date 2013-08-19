#ifndef    page_local_res_format_native_skeleton_hpp
#   define page_local_res_format_native_skeleton_hpp

#	include <cstdint> // {,u}int32_t

namespace page { namespace res { namespace format
{
	namespace native { namespace skeleton
	{
		const char sig[] = {'P', 'A', 'G', 'E', 's', 'k', 'e', 'l'};

#	pragma pack(push, 1)
		struct Header
		{
			std::uint32_t bones;
		};
		struct BoneName
		{
			std::uint32_t size;
		};
		struct Bone
		{
			float position[3];
			float orientation[3]; // quaternion.xyz, normalized w
			std::int32_t parent;
		};
#	pragma pack(pop)

		const char headerFormat[] = "d";
		const char boneNameFormat[] = "d";
		const char boneFormat[] = "ddddddd";
	}}

	using namespace native::skeleton;
}}}

#endif
