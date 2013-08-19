#ifndef    page_local_res_format_freetype_sub_hpp
#   define page_local_res_format_freetype_sub_hpp

#	include <string>
#	include <cstdint> // uint32_t

namespace page { namespace res { namespace format
{
	namespace freetype
	{
		const std::string mimeTypeForSub("page/font-freetype-sub");

#	pragma pack(push, 1)
		struct Header
		{
			std::uint32_t index;
		};
#	pragma pack(pop)
	}

	using namespace freetype;
}}}

#endif
