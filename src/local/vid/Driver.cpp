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
			wnd(wnd), scene(0), userInterface(0),
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
		void Driver::Imbue(const ui::UserInterface *userInterface)
		{
			this->userInterface = userInterface;
		}

		// inspiration access
		const phys::Scene *Driver::GetScene() const
		{
			return scene;
		}
		const ui::UserInterface *Driver::GetUserInterface() const
		{
			return userInterface;
		}
	}
}
