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

#include "../res/type/Theme.hpp" // Theme::{margin,scale}
#include "../vid/DrawContext.hpp" // DrawContext::{FrameSaver,GetFrameAspect,PushFrame}
#include "DrawContext.hpp"
#include "Root.hpp"
#include "Widget.hpp" // Widget::{Draw,Get{Position,Size},OnOver,Update}

namespace page { namespace gui
{
	/*-------------+
	| constructors |
	+-------------*/

	Root::Root(const std::shared_ptr<const res::Theme> &theme) :
		theme(theme) {}

	/*------+
	| theme |
	+------*/

	const res::Theme &Root::GetTheme() const
	{
		return *theme;
	}

	void Root::SetTheme(const std::shared_ptr<const res::Theme> &theme)
	{
		this->theme = theme;
	}

	/*--------+
	| metrics |
	+--------*/

	math::Aabb<2> Root::GetBounds(const Widget &widget, const vid::DrawContext &context) const
	{
		return GetBounds(widget, context.GetFrameAspect());
	}

	math::Aabb<2> Root::GetBounds(const Widget &widget, float aspect) const
	{
		Widget::Size widgetSize(widget.GetSize(*theme));
		math::Vec2
			scale(1 / (theme->scale * math::Vec2(aspect, 1))),
			size((widgetSize.min + theme->margin * 2) * scale);
		size = Select(widgetSize.mode == Widget::Size::max, Max(size, 1), size);
		math::Vec2 pos(widget.GetPosition() * (1 - size));
		return Shrink(AabbPositionSize(pos, size), theme->margin * scale);
	}

	// rendering
	void Root::Draw(vid::DrawContext &context) const
	{
		DrawContext interfaceContext(context, *theme);
		for (Widgets::const_iterator iter(widgets.begin()); iter != widgets.end(); ++iter)
		{
			const Widget &widget(**iter);
			vid::DrawContext::FrameSaver frameSaver(context);
			context.PushFrame(GetBounds(widget, context));
			widget.Draw(interfaceContext);
		}
	}

	// update
	void Root::Update(float deltaTime)
	{
		for (Widgets::iterator iter(widgets.begin()); iter != widgets.end(); ++iter)
		{
			Widget &widget(**iter);
			widget.Update(deltaTime);
		}
	}
	bool Root::UpdateCursor(const math::Vec2 &pos, float aspect)
	{
		bool result = false;
		for (Widgets::iterator iter(widgets.begin()); iter != widgets.end(); ++iter)
		{
			Widget &widget(**iter);
			math::Vec2 themePos(pos * theme->scale * math::Vec2(aspect, 1));
			math::Aabb<2> bounds(GetBounds(widget, aspect) * theme->scale * math::Vec2(aspect, 1));
			if (Contains(bounds, themePos))
				result |= widget.OnOver(themePos - bounds.min, *theme, Size(bounds));
		}
		return result;
	}
}}
