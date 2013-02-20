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

#include <algorithm> // max, min

#include "../math/interp.hpp" // HermiteScale
#include "../vid/DrawContext.hpp" // DrawContext::{alphaFilter,GetFilterCaps,PushAlphaFilter}
#include "DrawContext.hpp" // DrawContext::GetBase
#include "Widget.hpp"

namespace page
{
	namespace ui
	{
		// constants
		const float
			Widget::glowFadeInDuration  = .0625,
			Widget::glowFadeOutDuration = .5,
			Widget::glowFadeExponent    = 1.5,
			opacityFadeInDuration       = .5,
			opacityFadeOutDuration      = .5,
			opacityFadeExponent         = 1.5;

		// size construct
		WidgetSize::WidgetSize(const math::Vector<2> &min, const math::Vector<2, int> &mode) :
			min(min), mode(mode) {}

		// construct
		Widget::Widget(const math::Vector<2> &pos, const math::Vector<2> &minSize, bool visible) :
			pos(pos), minSize(minSize), visible(visible), opacity(visible) {}

		// attribute state
		const math::Vector<2> &Widget::GetPosition() const
		{
			return pos;
		}
		const math::Vector<2> &Widget::GetMinSize() const
		{
			return minSize;
		}

		// visibility state
		bool Widget::IsEnabled() const
		{
			return visible;
		}
		bool Widget::IsVisible() const
		{
			return opacity;
		}

		// metrics
		Widget::Size Widget::GetSize(const res::Theme &theme) const
		{
			Size size(CalcSize(theme));
			size.min = Max(size.min, minSize * theme.scale);
			return size;
		}

		// rendering
		void Widget::Draw(DrawContext &context) const
		{
			if (!opacity) return;
			DrawContext::Base::FilterSaver filterSaver(context.GetBase());
			if (opacity < 1)
			{
				if (context.GetBase().GetFilterCaps() & DrawContext::Base::alphaFilter)
					context.GetBase().PushAlphaFilter(math::HermiteScale(opacity, opacityFadeExponent));
				else
					// HACK: alpha compositing not supported; abort transition
					// and jump to target state immediately
					if (!(const_cast<Widget &>(*this).opacity = visible)) return;
			}
			DoDraw(context);
		}

		// update
		void Widget::Update(float deltaTime)
		{
			DoUpdate(deltaTime);
			// update opacity
			opacity = visible ?
				std::min(opacity + deltaTime / opacityFadeInDuration,  1.f) :
				std::max(opacity - deltaTime / opacityFadeOutDuration, 0.f);
		}

		// attribute modifiers
		void Widget::SetPosition(const math::Vector<2> &pos)
		{
			this->pos = pos;
		}
		void Widget::SetMinSize(const math::Vector<2> &minSize)
		{
			this->minSize = minSize;
		}

		// visibility modifiers
		void Widget::Show(bool fade)
		{
			visible = true;
			if (!fade) opacity = 1;
		}
		void Widget::Hide(bool fade)
		{
			visible = false;
			if (!fade) opacity = 0;
		}
		void Widget::SetVisible(bool visible, bool fade)
		{
			this->visible = visible;
			if (!fade) opacity = visible;
		}
		void Widget::SetOpacity(float opacity)
		{
			this->opacity = opacity;
		}

		// cursor event notification
		bool Widget::OnOver(const math::Vector<2> &, const res::Theme &, const math::Vector<2> &) {}
		bool Widget::OnDown(const math::Vector<2> &, const res::Theme &, const math::Vector<2> &) {}
		bool Widget::OnClick(const math::Vector<2> &, const res::Theme &, const math::Vector<2> &) {}

		// virtual update
		void Widget::DoUpdate(float) {}
	}
}
