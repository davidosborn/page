#ifndef    page_local_vid_opengl_resources_ShaderOutlineResources_hpp
#   define page_local_vid_opengl_resources_ShaderOutlineResources_hpp

#	include <memory> // unique_ptr

#	include "../../../math/fwd.hpp" // Vector
#	include "../../../util/class/special_member_functions.hpp" // Uncopyable

namespace page
{
	namespace vid
	{
		namespace opengl
		{
			class Program;
			class RenderTargetPool;

			struct ShaderOutlineResources : util::Uncopyable<ShaderOutlineResources>
			{
				// construct
				ShaderOutlineResources(const math::Vec2u &size);

				// access
				const Program &GetProgram() const;
				const RenderTargetPool &GetRenderTargetPool() const;

				// compatibility
				static bool Check();

				private:
				std::unique_ptr<Program> program;
				std::unique_ptr<RenderTargetPool> renderTargetPool;
			};
		}
	}
}

#endif
