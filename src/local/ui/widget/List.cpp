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

#include <algorithm> // min
#include "../../res/type/Font.hpp" // Font::lineHeight
#include "../../res/type/Theme.hpp" // GetThickness, Theme::{list,scale}
#include "../DrawContext.hpp"
#include "List.hpp"

namespace page
{
	namespace ui
	{
		// construct
		List::List(const math::Vector<2> &size) :
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

		// clone
		List *List::Clone() const
		{
			return new List(*this);
		}

		// modifiers
		void List::Insert(const std::string &item)
		{
			Items::difference_type index = selection - items.begin();
			items.push_back(item);
			selection = items.begin() + index;
		}

		// cursor event notification
		bool List::OnOver(const math::Vector<2> &pos, const res::Theme &theme, const math::Vector<2> &size)
		{
			// FIXME: implement
		}
		bool List::OnClick(const math::Vector<2> &pos, const res::Theme &theme, const math::Vector<2> &size)
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
			math::Vector<2> scale(context.GetScale());
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
						math::Vector<2>(0, index * (lineHeight + halfLineGap)),
						math::Vector<2>(1, lineHeight + halfLineGap)));
			}
			// draw items
			math::Vector<2> pen(0, halfLineGap);
			for (Items::const_iterator item(items.begin());
				item != items.end() && pen.y <= 1; ++item)
			{
				math::Aabb<2> aabb(pen, pen + math::Vector<2>(1, lineHeight));
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
	}
}
