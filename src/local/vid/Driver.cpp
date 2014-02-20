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
