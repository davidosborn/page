#ifndef    page_local_res_format_native_track_hpp
#   define page_local_res_format_native_track_hpp

#	include <cstdint> // {,u}int32_t

namespace page { namespace res { namespace format
{
	namespace native { namespace track
	{
		const char sig[] = {'P', 'A', 'G', 'E', 't', 'r', 'a', 'c', 'k'};

#	pragma pack(push, 1)
		struct Header
		{
			std::uint32_t faces;
		};
		struct Face
		{
			float vertices[3][3];
			std::int32_t neighbours[3];
		};
#	pragma pack(pop)

		const char headerFormat[] = "d";
		const char faceFormat[] = "dddddddddddd";
	}}

	using namespace native::track;
}}}

#endif
