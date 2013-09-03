#include <memory> // unique_ptr

#include "../cfg/vars.hpp"
#include "../phys/Camera.hpp" // Camera::GetOpacity, GetViewFrustum
#include "../phys/Scene.hpp" // Scene::GetCameras
#include "../gui/UserInterface.hpp" // UserInterface::Draw
#include "DrawContext.hpp" // DrawContext::{{alpha,median}Filter,GetFilterCaps,MakeViewContext,Push{Alpha,Median}Filter,ScaleBias}
#include "ViewContext.hpp" // ViewContext::Draw

namespace page
{
	namespace vid
	{
		void Draw(DrawContext &, const phys::Scene &, const phys::Camera &);

		// interface rendering
		void Draw(DrawContext &context, const ui::UserInterface &ui)
		{
			ui.Draw(context);
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
			if (*CVAR(opengl)::renderMedian &&
				context.GetFilterCaps() & DrawContext::medianFilter)
				context.PushMedianFilter(*CVAR(opengl)::renderMedianLevel, true);
			const std::unique_ptr<ViewContext> viewContext(context.MakeViewContext(GetViewFrustum(camera)));
			viewContext->Draw(scene);
		}
	}
}
