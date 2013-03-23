/**
 * @section license
 *
 * Copyright (c) 2006-2013 David Osborn
 *
 * Permission is granted to use and redistribute this software in source and
 * binary form, with or without modification, subject to the following
 * conditions:
 *
 * 1. Redistributions in source form must retain the above copyright notice,
 *    this list of conditions, and the following disclaimer.
 *
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

#include <algorithm> // ceil
#include <cassert>
#include <memory> // unique_ptr

#include "../math/Vector.hpp"
#include "draw.hpp" // Draw
#include "DrawContext.hpp" // DrawContext::{~DrawContext,FilterSaver,GetFilterCaps,PushSaturationFilter,saturationFilter,ScaleBias}
#include "Driver.hpp"

namespace page
{
	namespace vid
	{
		// construct/destroy
		Driver::Driver(wnd::Window &wnd) :
			wnd(wnd), scene(0), interface(0),
			sceneSaturation(1), brightness(0), contrast(1) {}
		Driver::~Driver() {}

		// update
		void Driver::Update()
		{
			Render();
			Flush();
		}

		// rendering
		void Driver::Render(const math::Aabb<2> &logicalBox)
		{
			if (!(All(Size(logicalBox)) && (scene || interface))) return;
			const std::unique_ptr<DrawContext> context(MakeDrawContext(logicalBox));
			if (scene)
			{
				DrawContext::FilterSaver filterSaver(*context);
				if (sceneSaturation < 1 && context->GetFilterCaps() & DrawContext::saturationFilter)
					context->PushSaturationFilter(sceneSaturation, true);
				Draw(*context, *scene);
			}
			if (interface)
				Draw(*context, *interface);
			if (brightness || contrast != 1)
				context->ScaleBias(brightness, contrast);
		}

		// scene modifiers
		void Driver::SetSceneSaturation(float saturation)
		{
			sceneSaturation = saturation;
		}

		// color correction
		float Driver::GetBrightness() const
		{
			return brightness;
		}
		float Driver::GetContrast() const
		{
			return contrast;
		}
		void Driver::SetBrightness(float brightness)
		{
			this->brightness = brightness;
		}
		void Driver::SetContrast(float contrast)
		{
			this->contrast = contrast;
		}

		// window access
		wnd::Window &Driver::GetWindow()
		{
			return wnd;
		}
		const wnd::Window &Driver::GetWindow() const
		{
			return wnd;
		}

		// inspiration modifiers
		void Driver::Imbue(const phys::Scene *scene)
		{
			this->scene = scene;
		}
		void Driver::Imbue(const ui::Interface *interface)
		{
			this->interface = interface;
		}

		// inspiration access
		const phys::Scene *Driver::GetScene() const
		{
			return scene;
		}
		const ui::Interface *Driver::GetInterface() const
		{
			return interface;
		}
	}
}
