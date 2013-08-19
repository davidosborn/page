#ifndef    page_local_res_format_native_character_hpp
#   define page_local_res_format_native_character_hpp

#	include <string>

namespace page { namespace res { namespace format
{
	namespace native { namespace character
	{
		const char comment = '#';
		const std::string shebang(std::string(1, comment) + '!');
		const std::string sig("PAGEchar");
	}}

	using namespace native::character;
}}}

#endif
