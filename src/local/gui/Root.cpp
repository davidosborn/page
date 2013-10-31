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
