#ifndef    page_local_res_format_native_theme_hpp
#   define page_local_res_format_native_theme_hpp

#	include <string>

namespace page { namespace res { namespace format
{
	namespace native { namespace theme
	{
		const char comment = '#';
		const std::string shebang(std::string(1, comment) + '!');
		const std::string sig("PAGEtheme");
	}}

	using namespace native::theme;
}}}

#endif
