#include "../../res/type/Image.hpp" // Image::size
#include "../../res/type/Theme.hpp"
#include "../DrawContext.hpp"
#include "ButtonWidget.hpp"

namespace page { namespace gui
{
	/*-----------------+
	| Widget overrides |
	+-----------------*/

	WidgetSize ButtonWidget::CalcSize(const res::Theme &theme) const
	{
		// FIXME: implement
		return WidgetSize(0);
	}

	void ButtonWidget::DoDraw(DrawContext &context) const
	{
		// FIXME: implement
	}
}}
