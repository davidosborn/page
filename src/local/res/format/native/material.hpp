#ifndef    page_local_res_format_native_material_hpp
#   define page_local_res_format_native_material_hpp

#	include <string>

namespace page { namespace res { namespace format
{
	namespace native { namespace material
	{
		const char comment = '#';
		const std::string shebang(std::string(1, comment) + '!');
		const std::string sig("PAGEmat");
	}}

	using namespace native::material;
}}}

#endif
