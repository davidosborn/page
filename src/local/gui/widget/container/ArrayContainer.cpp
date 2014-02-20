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

#include <algorithm> // max
#include <functional> // bind

#include "../../../res/type/Theme.hpp" // Theme::margin
#include "../../../vid/DrawContext.hpp" // DrawContext::{FrameSaver,PushFrame}
#include "../../DrawContext.hpp"
#include "ArrayContainer.hpp"

namespace page { namespace gui
{
	// construct
	ArrayContainer::ArrayContainer(bool horizontal, bool margin) :
		horizontal(horizontal), margin(margin) {}

	// cursor event notification
	bool ArrayContainer::OnOver(const math::Vec2 &pos, const res::Theme &theme, const math::Vec2 &size)
	{
		using namespace std::placeholders;
		return CheckHit(pos, theme, size,
			std::bind(&Widget::OnOver, _1, _2, theme, _3));
	}
	bool ArrayContainer::OnDown(const math::Vec2 &pos, const res::Theme &theme, const math::Vec2 &size)
	{
		using namespace std::placeholders;
		return CheckHit(pos, theme, size,
			std::bind(&Widget::OnDown, _1, _2, theme, _3));
	}
	bool ArrayContainer::OnClick(const math::Vec2 &pos, const res::Theme &theme, const math::Vec2 &size)
	{
		using namespace std::placeholders;
		return CheckHit(pos, theme, size,
			std::bind(&Widget::OnClick, _1, _2, theme, _3));
	}

	// metrics
	auto ArrayContainer::CalcSize(const res::Theme &theme) const -> WidgetSize
	{
		// FIXME: support different sizing modes
		WidgetSize size;
		if (!widgets.empty())
			for (Widgets::const_iterator iter(widgets.begin());;)
			{
				const Widget &child(**iter);
				auto childSize(child.GetSize(theme));
				size.min[!horizontal] += childSize.min[!horizontal];
				size.min[horizontal] = std::max(childSize.min[horizontal], size.min[horizontal]);
				if (++iter == widgets.end()) break;
				if (margin) size.min[!horizontal] += theme.margin;
			}
		return size;
	}

	// rendering
	void ArrayContainer::DoDraw(DrawContext &context) const
	{
		const res::Theme &theme(context.GetTheme());
		math::Vec2 scale(context.GetScale());
		// build child size array
		typedef std::vector<math::Vec2> Sizes;
		Sizes sizes;
		sizes.reserve(widgets.size());
		for (Widgets::const_iterator iter(widgets.begin()); iter != widgets.end(); ++iter)
		{
			const Widget &child(**iter);
			sizes.push_back(child.GetSize(theme).min * scale);
		}
		// calculate child positions from size modes
		// FIXME: implement; support different sizing modes
		// draw children
		math::Vec2 pen;
		Sizes::const_iterator size(sizes.begin());
		for (Widgets::const_iterator iter(widgets.begin()); iter != widgets.end(); ++iter, ++size)
		{
			const Widget &child(**iter);
			DrawContext::Base::FrameSaver frameSaver(context.GetBase());
			context.GetBase().PushFrame(AabbPositionSize(pen, *size));
			child.Draw(context);
			pen[!horizontal] += (*size)[!horizontal];
			if (margin) pen[!horizontal] += theme.margin * scale[!horizontal];
		}
	}

	// update
	void ArrayContainer::DoUpdate(float deltaTime)
	{
		for (Widgets::iterator iter(widgets.begin()); iter != widgets.end(); ++iter)
		{
			Widget &child(**iter);
			child.Update(deltaTime);
		}
	}

	// child hit detection
	bool ArrayContainer::CheckHit(const math::Vec2 &pos, const res::Theme &theme, const math::Vec2 &size, const HitCallback &cb)
	{
		bool result = false;
		math::Vec2 pen;
		for (Widgets::const_iterator iter(widgets.begin()); iter != widgets.end(); ++iter)
		{
			Widget &child(**iter);
			auto childSize(child.GetSize(theme));
			// FIXME: support different sizing modes
			math::Aabb<2> aabb(AabbPositionSize(pen, childSize.min));
			if (Contains(aabb, pos))
				result |= cb(&child, pos - aabb.min, math::Size(aabb));
			pen[!horizontal] += childSize.min[!horizontal];
			if (margin) pen[!horizontal] += theme.margin;
		}
		return result;
	}
}}
