#ifndef    page_local_res_format_native_animation_hpp
#   define page_local_res_format_native_animation_hpp

#	include <cstdint> // uint32_t

namespace page { namespace res { namespace format
{
	namespace native { namespace animation
	{
		const char sig[] = {'P', 'A', 'G', 'E', 'a', 'n', 'i', 'm'};

#	pragma pack(push, 1)
		struct Header
		{
			float duration;
			std::uint32_t bones;
		};
		struct Bone
		{
			std::uint32_t nameSize;
			std::uint32_t frames;
		};
		struct Frame
		{
			float time;
			float position[3];
			float orientation[4];
			float scale[3];
		};
#	pragma pack(pop)

		const char headerFormat[] = "dd";
		const char boneFormat[] = "dd";
		const char frameFormat[] = "ddddddddddd";
	}}

	using namespace native::animation;
}}}

#endif
