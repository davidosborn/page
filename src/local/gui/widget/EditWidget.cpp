#include "EditWidget.hpp"

namespace page { namespace ui
{
	/*-----------------+
	| Widget overrides |
	+-----------------*/

	WidgetSize EditWidget::CalcSize(const res::Theme &theme) const
	{
		// FIXME: implement
		return WidgetSize(0);
	}

	void EditWidget::DoDraw(DrawContext &context) const
	{
		// FIXME: implement
		//context.DrawText("test", res::Resource<res::Font>("default.ttf"), math::Vec2(), 1.f, 0.f);
	}
}}
