#include <algorithm> // min

#include "../../res/type/Font.hpp" // Font::lineHeight
#include "../../res/type/Theme.hpp" // GetThickness, Theme::{list,scale}
#include "../DrawContext.hpp"
#include "List.hpp"

namespace page { namespace ui
{
	// construct
	List::List(const math::Vec2 &size) :
		size(size), selection(items.end()) {}
	List::List(const List &other) : Widget(other),
		size(other.size), items(other.items),
		selection(items.begin() + (other.selection - other.items.begin())) {}
	List &List::operator =(const List &other)
	{
		Widget::operator =(other);
		size = other.size;
		items = other.items;
		selection = items.begin() + (other.selection - other.items.begin());
		return *this;
	}

	// modifiers
	void List::Insert(const std::string &item)
	{
		Items::difference_type index = selection - items.begin();
		items.push_back(item);
		selection = items.begin() + index;
	}

	// cursor event notification
	bool List::OnOver(const math::Vec2 &pos, const res::Theme &theme, const math::Vec2 &size)
	{
		// FIXME: implement
	}
	bool List::OnClick(const math::Vec2 &pos, const res::Theme &theme, const math::Vec2 &size)
	{
		// FIXME: implement
	}

	// metrics
	List::Size List::CalcSize(const res::Theme &theme) const
	{
		// FIXME: when size is zero, calculate it using the maximum line
		// width and number of list items
		return Size(Select(size, size * theme.scale, 0), Size::grow);
	}

	// rendering
	void List::DoDraw(DrawContext &context) const
	{
		const res::Theme &theme(context.GetTheme());
		math::Vec2 scale(context.GetScale());
		// calculate corner coordinates
		math::Aabb<2>
			frameThickness(GetThickness(theme.list.frame) * scale),
			cornerBox(Shrink(math::Aabb<2>(0, 1),
				frameThickness.min / 2,
				frameThickness.max / 2));
		// FIXME: implement
		// draw background
		// FIXME: implement
		// draw background decorations
		// FIXME: implement
		// calculate text metrics
		const res::Font &font(*theme.list.text.font);
		float
			lineHeight = font.lineHeight * theme.list.text.size * scale.y,
			maxBearing = font.maxBearing.y * theme.list.text.size * scale.y,
			halfLineGap = (lineHeight - maxBearing) / 2;
		// draw highlight
		if (selection != items.end())
		{
			Items::difference_type index = selection - items.begin();
			context.DrawBackground(theme.list.highlight,
				AabbPositionSize(
					math::Vec2(0, index * (lineHeight + halfLineGap)),
					math::Vec2(1, lineHeight + halfLineGap)));
		}
		// draw items
		math::Vec2 pen(0, halfLineGap);
		for (Items::const_iterator item(items.begin());
			item != items.end() && pen.y <= 1; ++item)
		{
			math::Aabb<2> aabb(pen, pen + math::Vec2(1, lineHeight));
			aabb.max.y = std::min(aabb.max.y, 1.f);
			context.DrawText(theme.list.text, *item, aabb,
				math::WhiteRgbaColor(), false, res::centerTextAlign);
			pen.y = aabb.max.y + halfLineGap;
		}
		// draw frame
		// FIXME: implement
		// draw foreground decorations
		// FIXME: implement
	}
}}
