#ifndef    page_local_res_format_native_object_hpp
#   define page_local_res_format_native_object_hpp

#	include <string>

namespace page { namespace res { namespace format
{
	namespace native { namespace object
	{
		const char comment = '#';
		const std::string shebang(std::string(1, comment) + '!');
		const std::string sig("PAGEobj");
	}}

	using namespace native::object;
}}}

#endif
