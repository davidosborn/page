#ifndef    page_local_res_format_native_scene_hpp
#   define page_local_res_format_native_scene_hpp

#	include <string>

namespace page { namespace res { namespace format
{
	namespace native { namespace scene
	{
		const char comment = '#';
		const std::string shebang(std::string(1, comment) + '!');
		const std::string sig("PAGEscene");
	}}

	using namespace native::scene;
}}}

#endif
