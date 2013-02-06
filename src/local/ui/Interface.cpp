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

#include "../res/type/Theme.hpp" // Theme::{margin,scale}
#include "../vid/DrawContext.hpp" // DrawContext::{FrameSaver,GetFrameAspect,PushFrame}
#include "DrawContext.hpp"
#include "Interface.hpp"
#include "Widget.hpp" // Widget::{Draw,Get{Position,Size},OnOver,Update}

namespace page
{
	namespace ui
	{
		// construct/destroy
		Interface::Interface(const std::shared_ptr<const res::Theme> &theme) :
			theme(theme) {}
		Interface::~Interface() {}

		// metrics
		math::Aabb<2> Interface::GetBounds(const Widget &widget, const vid::DrawContext &context) const
		{
			return GetBounds(widget, context.GetFrameAspect());
		}
		math::Aabb<2> Interface::GetBounds(const Widget &widget, float aspect) const
		{
			Widget::Size widgetSize(widget.GetSize(*theme));
			math::Vector<2>
				scale(1 / (theme->scale * math::Vector<2>(aspect, 1))),
				size((widgetSize.min + theme->margin * 2) * scale);
			size = Select(widgetSize.mode == Widget::Size::max, Max(size, 1), size);
			math::Vector<2> pos(widget.GetPosition() * (1 - size));
			return Shrink(AabbPositionSize(pos, size), theme->margin * scale);
		}

		// theme access
		const res::Theme &Interface::GetTheme() const
		{
			return *theme;
		}
		void Interface::SetTheme(const std::shared_ptr<const res::Theme> &theme)
		{
			this->theme = theme;
		}

		// rendering
		void Interface::Draw(vid::DrawContext &context) const
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
		void Interface::Update(float deltaTime)
		{
			for (Widgets::iterator iter(widgets.begin()); iter != widgets.end(); ++iter)
			{
				Widget &widget(**iter);
				widget.Update(deltaTime);
			}
		}
		bool Interface::UpdateCursor(const math::Vector<2> &pos, float aspect)
		{
			bool result = false;
			for (Widgets::iterator iter(widgets.begin()); iter != widgets.end(); ++iter)
			{
				Widget &widget(**iter);
				math::Vector<2> themePos(pos * theme->scale * math::Vector<2>(aspect, 1));
				math::Aabb<2> bounds(GetBounds(widget, aspect) * theme->scale * math::Vector<2>(aspect, 1));
				if (Contains(bounds, themePos))
					result |= widget.OnOver(themePos - bounds.min, *theme, Size(bounds));
			}
			return result;
		}
	}
}
