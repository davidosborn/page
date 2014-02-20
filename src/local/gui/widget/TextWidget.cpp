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

#include "../../res/type/Font.hpp" // GetTextSize
#include "../../res/type/Theme.hpp" // Theme::{scale,text}
#include "../DrawContext.hpp"
#include "TextWidget.hpp"

namespace page { namespace gui
{
	/*-------------+
	| constructors |
	+-------------*/

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
			WidgetSize::Mode::grow);
	}

	void TextWidget::DoDraw(DrawContext &context) const
	{
		context.DrawText(context.GetTheme().text, text, math::Aabb<2>(0, 1),
			color, wrap, wrap ? res::justifyTextAlign : res::leftTextAlign);
	}
}}
