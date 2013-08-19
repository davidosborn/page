#include "ViewContext.hpp"

namespace page
{
	namespace vid
	{
		// construct/destroy
		ViewContext::ViewContext(Base &base, const math::ViewFrustum<> &frustum) :
			base(base), frustum(frustum), frameSaver(base), clipSaver(base)
		{
			// clear frame
			Base::ClipSaver clipSaver(base);
			base.PushClip(math::Aabb<2>(0, 1));
			base.Clear();
			clipSaver.Reset();
			// setup aspect ratio
			float aspect = base.GetFrameAspect();
			if (aspect != frustum.aspect)
			{
				base.PushFrame(math::Aabb<2>(-.5, .5) * math::Vec2(
					std::min(frustum.aspect / aspect, 1.f),
					std::min(aspect / frustum.aspect, 1.f)) + .5f);
				base.PushClip(math::Aabb<2>(0, 1));
			}
		}
		ViewContext::~ViewContext() {}

		// base context access
		ViewContext::Base &ViewContext::GetBase()
		{
			return base;
		}
		const ViewContext::Base &ViewContext::GetBase() const
		{
			return base;
		}

		// view frustum access
		const math::ViewFrustum<> &ViewContext::GetFrustum() const
		{
			return frustum;
		}
	}
}
