/**
 * @section license
 *
 * Copyright (c) 2006-2012 David Osborn
 *
 * Permission is granted to use and redistribute this software in source and
 * binary form, with or without modification, subject to the following
 * conditions:
 *
 * 1. Redistributions in source form must retain the above copyright notice,
 *    this list of conditions, and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions, and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution, and in the same
 *    place and form as other copyright, license, and disclaimer information.
 *
 * As a special exception, distributions of derivative works in binary form may
 * include an acknowledgement in place of the above copyright notice, this list
 * of conditions, and the following disclaimer in the documentation and/or other
 * materials provided with the distribution, and in the same place and form as
 * other acknowledgements, similar in substance to the following:
 *
 *    Portions of this software are based on the work of David Osborn.
 *
 * This software is provided "as is", without any express or implied warranty.
 * In no event will the authors be liable for any damages arising out of the use
 * of this software.
 */

#include "../cfg/opengl.hpp" // vidMedian{,Level}
#include "../phys/Camera.hpp" // Camera::GetOpacity, GetViewFrustum
#include "../phys/Scene.hpp" // Scene::GetCameras
#include "../ui/Interface.hpp" // Interface::Draw
#include "../util/scoped_ptr.hpp"
#include "DrawContext.hpp" // DrawContext::{{alpha,median}Filter,GetFilterCaps,MakeViewContext,Push{Alpha,Median}Filter,ScaleBias}
#include "ViewContext.hpp" // ViewContext::Draw

namespace page
{
	namespace vid
	{
		void Draw(DrawContext &, const phys::Scene &, const phys::Camera &);

		// interface rendering
		void Draw(DrawContext &context, const ui::Interface &interface)
		{
			interface.Draw(context);
		}

		// scene rendering
		void Draw(DrawContext &context, const phys::Scene &scene)
		{
			typedef phys::Scene::View<phys::Camera>::Type Cameras;
			Cameras cameras(scene.GetCameras());
			if (cameras.empty()) return;
			if (context.GetFilterCaps() & DrawContext::alphaFilter)
			{
				for (Cameras::const_iterator iter(cameras.begin()); iter != cameras.end(); ++iter)
				{
					const phys::Camera &camera(**iter);
					DrawContext::FilterSaver filterSaver(context);
					if (camera.GetOpacity() < 1)
						context.PushAlphaFilter(camera.GetOpacity(), true);
					Draw(context, scene, camera);
				}
			}
			else
			{
				float opacity = cameras.back()->GetOpacity();
				if (opacity < 1)
				{
					if (cameras.size() > 1)
					{
						if (opacity < .5f)
						{
							Draw(context, scene, *cameras.end()[-2]);
							context.ScaleBias(1 - opacity * 2, 0);
						}
						else
						{
							Draw(context, scene, *cameras.back());
							context.ScaleBias(opacity * 2 - 1, 0);
						}
					}
					else
					{
						Draw(context, scene, *cameras.back());
						context.ScaleBias(opacity, 0);
					}
				}
				else Draw(context, scene, *cameras.back());
			}
		}
		void Draw(DrawContext &context, const phys::Scene &scene, const phys::Camera &camera)
		{
			DrawContext::FilterSaver filterSaver(context);
			// FIXME: scale/bias for exposure
			// FIXME: post-processing effects, such as rain on camera lens
			if (*cfg::opengl::vidMedian &&
				context.GetFilterCaps() & DrawContext::medianFilter)
				context.PushMedianFilter(*cfg::opengl::vidMedianLevel, true);
			util::scoped_ptr<ViewContext> viewContext(context.MakeViewContext(GetViewFrustum(camera)));
			viewContext->Draw(scene);
		}
	}
}
