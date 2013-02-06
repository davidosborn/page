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

#include "../../res/type/Font.hpp" // GetTextSize
#include "../../res/type/Theme.hpp" // Theme::{scale,text}
#include "../DrawContext.hpp"
#include "Text.hpp"

namespace page
{
	namespace ui
	{
		// construct
		Text::Text(const std::string &text, const math::Vector<2> &size,
			const math::RgbaColor<> &color, bool wrap) :
			text(text), size(size), color(color), wrap(wrap && size.x) {}

		// clone
		Text *Text::Clone() const
		{
			return new Text(*this);
		}

		// modifiers
		void Text::SetText(const std::string &text)
		{
			this->text = text;
		}

		// metrics
		Text::Size Text::CalcSize(const res::Theme &theme) const
		{
			return Size(Select(size, size * theme.scale,
				GetTextSize(*theme.text.font, text.begin(), text.end(),
					wrap ? size.x * theme.scale : 0) * theme.text.size),
				Size::grow);
		}

		// rendering
		void Text::DoDraw(DrawContext &context) const
		{
			context.DrawText(context.GetTheme().text, text, math::Aabb<2>(0, 1),
				color, wrap, wrap ?
					res::justifyTextAlign :
					res::leftTextAlign);
		}
	}
}
