#include "../../res/type/Font.hpp" // GetTextSize
#include "../../res/type/Theme.hpp" // Theme::{scale,text}
#include "../DrawContext.hpp"
#include "TextWidget.hpp"

namespace page { namespace ui
{
	/*--------------------------+
	| constructors & destructor |
	+--------------------------*/

	TextWidget::TextWidget(
		std::string       const& text,
		math::Vec2        const& size,
		math::RgbaColor<> const& color,
		bool                     wrap) :
			text(text), size(size), color(color),
			wrap(wrap && size.x) {}

	/*-----------+
	| properties |
	+-----------*/

	const std::string &TextWidget::GetText() const
	{
		return text;
	}

	void TextWidget::SetText(const std::string &text)
	{
		this->text = text;
	}

	/*-----------------+
	| Widget overrides |
	+-----------------*/

	WidgetSize TextWidget::CalcSize(const res::Theme &theme) const
	{
		return WidgetSize(Select(size, size * theme.scale,
			GetTextSize(*theme.text.font, text.begin(), text.end(),
				wrap ? size.x * theme.scale : 0) * theme.text.size),
			Size::grow);
	}

	void TextWidget::DoDraw(DrawContext &context) const
	{
		context.DrawText(context.GetTheme().text, text, math::Aabb<2>(0, 1),
			color, wrap, wrap ? res::justifyTextAlign : res::leftTextAlign);
	}
}}
