/**
 * @copyright
 *
 * Copyright (c) 2006-2014 David Osborn
 *
 * Permission is granted to use and redistribute this software in source and
 * binary form, with or without modification, subject to the following
 * conditions:
 *
 * 1. Redistributions in source form must retain the above copyright notice,
 *    this list of conditions, and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions, and the following disclaimer in the same place
 *    and form as other copyright, license, and disclaimer information.
 *
 * 3. Redistributions in binary form must also include an acknowledgement in the
 *    same place and form as other acknowledgements (such as the credits),
 *    similar in substance to the following:
 *
 *       Portions of this software are based on the work of David Osborn.
 *
 * This software is provided "as is", without any express or implied warranty.
 * In no event will the authors be liable for any damages arising out of the use
 * of this software.
 */

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
