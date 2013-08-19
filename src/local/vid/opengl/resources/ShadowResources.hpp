#ifndef    page_local_vid_opengl_resources_ShadowResources_hpp
#   define page_local_vid_opengl_resources_ShadowResources_hpp

#	include <memory> // unique_ptr

#	include "../../../cfg/vars.hpp"
#	include "../../../util/class/special_member_functions.hpp" // Uncopyable

namespace page
{
	namespace vid
	{
		namespace opengl
		{
			class Program;
			class RenderTargetPool;

			struct ShadowResources : util::Uncopyable<ShadowResources>
			{
				// construct
				ShadowResources();

				// access
				enum Type
				{
					exponentialType = cfg::opengl::exponentialShadowType,
					packedType      = cfg::opengl::packedShadowType,
					varianceType    = cfg::opengl::varianceShadowType
				};
				Type GetType() const;
				const Program &GetProgram() const;
				const RenderTargetPool &GetRenderTargetPool() const;

				// compatibility
				static bool Check();

				private:
				Type type;
				std::unique_ptr<Program> program;
				std::unique_ptr<RenderTargetPool> renderTargetPool;
			};
		}
	}
}

#endif
