#include <algorithm> // max
#include <functional> // bind

#include "../../res/type/Theme.hpp" // Theme::margin
#include "../../vid/DrawContext.hpp" // DrawContext::{FrameSaver,PushFrame}
#include "../DrawContext.hpp"
#include "Array.hpp"

namespace page { namespace ui
{
	// construct
	Array::Array(bool horizontal, bool margin) :
		horizontal(horizontal), margin(margin) {}

	// cursor event notification
	bool Array::OnOver(const math::Vec2 &pos, const res::Theme &theme, const math::Vec2 &size)
	{
		using namespace std::placeholders;
		return CheckHit(pos, theme, size,
			std::bind(&Widget::OnOver, _1, _2, theme, _3));
	}
	bool Array::OnDown(const math::Vec2 &pos, const res::Theme &theme, const math::Vec2 &size)
	{
		using namespace std::placeholders;
		return CheckHit(pos, theme, size,
			std::bind(&Widget::OnDown, _1, _2, theme, _3));
	}
	bool Array::OnClick(const math::Vec2 &pos, const res::Theme &theme, const math::Vec2 &size)
	{
		using namespace std::placeholders;
		return CheckHit(pos, theme, size,
			std::bind(&Widget::OnClick, _1, _2, theme, _3));
	}

	// metrics
	Array::Size Array::CalcSize(const res::Theme &theme) const
	{
		// FIXME: support different sizing modes
		Size size;
		if (!widgets.empty())
			for (Widgets::const_iterator iter(widgets.begin());;)
			{
				const Widget &child(**iter);
				Size childSize(child.GetSize(theme));
				size.min[!horizontal] += childSize.min[!horizontal];
				size.min[horizontal] = std::max(childSize.min[horizontal], size.min[horizontal]);
				if (++iter == widgets.end()) break;
				if (margin) size.min[!horizontal] += theme.margin;
			}
		return size;
	}

	// rendering
	void Array::DoDraw(DrawContext &context) const
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
	void Array::DoUpdate(float deltaTime)
	{
		for (Widgets::iterator iter(widgets.begin()); iter != widgets.end(); ++iter)
		{
			Widget &child(**iter);
			child.Update(deltaTime);
		}
	}

	// child hit detection
	bool Array::CheckHit(const math::Vec2 &pos, const res::Theme &theme, const math::Vec2 &size, const HitCallback &cb)
	{
		bool result = false;
		math::Vec2 pen;
		for (Widgets::const_iterator iter(widgets.begin()); iter != widgets.end(); ++iter)
		{
			Widget &child(**iter);
			Size childSize(child.GetSize(theme));
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
