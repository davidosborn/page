#ifndef    page_local_res_format_native_model_hpp
#   define page_local_res_format_native_model_hpp

#	include <string>

namespace page { namespace res { namespace format
{
	namespace native { namespace model
	{
		const char comment = '#';
		const std::string shebang(std::string(1, comment) + '!');
		const std::string sig("PAGEmodel");
	}}

	using namespace native::model;
}}}

#endif
