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
