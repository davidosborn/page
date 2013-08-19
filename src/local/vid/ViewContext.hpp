#ifndef    page_local_vid_ViewContext_hpp
#   define page_local_vid_ViewContext_hpp

#	include "../math/ViewFrustum.hpp"
#	include "DrawContext.hpp" // {Frame,Clip}Saver

namespace page
{
	namespace phys { class Scene; }

	namespace vid
	{
		struct ViewContext
		{
			typedef DrawContext Base;

			// construct/destroy
			ViewContext(Base &, const math::ViewFrustum<> &);
			virtual ~ViewContext();

			// base context access
			Base &GetBase();
			const Base &GetBase() const;

			// view frustum access
			const math::ViewFrustum<> &GetFrustum() const;

			// scene rendering
			virtual void Draw(const phys::Scene &) = 0;

			private:
			Base &base;
			math::ViewFrustum<> frustum;
			DrawContext::FrameSaver frameSaver;
			DrawContext::ClipSaver clipSaver;
		};
	}
}

#endif
