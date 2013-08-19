#ifndef    page_local_res_format_native_gait_hpp
#   define page_local_res_format_native_gait_hpp

#	include <string>

namespace page { namespace res { namespace format
{
	namespace native { namespace gait
	{
		const char comment = '#';
		const std::string shebang(std::string(1, comment) + '!');
		const std::string sig("PAGEgait");
	}}

	using namespace native::gait;
}}}

#endif
