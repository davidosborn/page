#ifndef    page_local_res_format_native_cursor_hpp
#   define page_local_res_format_native_cursor_hpp

#	include <string>

namespace page { namespace res { namespace format
{
	namespace native { namespace cursor
	{
		const char comment = '#';
		const std::string shebang(std::string(1, comment) + '!');
		const std::string sig("PAGEcursor");
	}}

	using namespace native::cursor;
}}}

#endif
