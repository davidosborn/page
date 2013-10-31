#include <algorithm> // ceil
#include <cassert>
#include <memory> // unique_ptr

#include "../math/Vector.hpp"
#include "draw.hpp" // Draw
#include "DrawContext.hpp" // DrawContext::{~DrawContext,FilterSaver,GetFilterCaps,PushSaturationFilter,saturationFilter,ScaleBias}
#include "Driver.hpp"

namespace page { namespace vid
{
	/*-------------+
	| constructors |
	+-------------*/

	Driver::Driver(wnd::Window &window) :
		window(window) {}

	/*----------+
	| rendering |
	+----------*/

	void Driver::Render(const math::Aabb<2> &logicalBox)
	{
		if (!(All(Size(logicalBox)) && (scene || userInterface))) return;
		const std::unique_ptr<DrawContext> context(MakeDrawContext(logicalBox));
		if (scene)
		{
			DrawContext::FilterSaver filterSaver(*context);
			if (sceneSaturation < 1 && context->GetFilterCaps() & DrawContext::saturationFilter)
				context->PushSaturationFilter(sceneSaturation, true);
			Draw(*context, *scene);
		}
		if (userInterface)
			Draw(*context, *userInterface);
		if (brightness || contrast != 1)
			context->ScaleBias(brightness, contrast);
		Flush();
	}

	res::Image Driver::RenderImage(const math::Vec2u &size)
	{
		return DoRenderImage(size);
	}

	/*-----------------+
	| color correction |
	+-----------------*/

	float Driver::GetSceneSaturation()
	{
		return sceneSaturation;
	}

	void Driver::SetSceneSaturation(float saturation)
	{
		sceneSaturation = saturation;
	}

	float Driver::GetBrightness() const
	{
		return brightness;
	}

	void Driver::SetBrightness(float brightness)
	{
		this->brightness = brightness;
	}

	float Driver::GetContrast() const
	{
		return contrast;
	}

	void Driver::SetContrast(float contrast)
	{
		this->contrast = contrast;
	}

	/*--------------+
	| window access |
	+--------------*/

	wnd::Window &Driver::GetWindow()
	{
		return window;
	}

	const wnd::Window &Driver::GetWindow() const
	{
		return window;
	}
}}
