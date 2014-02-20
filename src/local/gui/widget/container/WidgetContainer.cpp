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

#include <cassert>

#include "../Widget.hpp" // Widget::Set{Clone,MinSize,Position,Visible}
#include "WidgetContainer.hpp"

namespace page { namespace gui
{
	// modifiers
	void WidgetContainer::Insert(const Widget &widget)
	{
		std::shared_ptr<Widget> clone(widget.Clone());
		widgets.push_back(clone);
	}
	void WidgetContainer::Insert(const Widget &widget, const math::Vec2 &pos)
	{
		std::shared_ptr<Widget> clone(widget.Clone());
		clone->SetPosition(pos);
		widgets.push_back(clone);
	}
	void WidgetContainer::Insert(const Widget &widget, const math::Vec2 &pos, const math::Vec2 &minSize)
	{
		std::shared_ptr<Widget> clone(widget.Clone());
		clone->SetPosition(pos);
		clone->SetMinSize(minSize);
		widgets.push_back(clone);
	}
	void WidgetContainer::Insert(const Widget &widget, const math::Vec2 &pos, const math::Vec2 &minSize, bool visible)
	{
		std::shared_ptr<Widget> clone(widget.Clone());
		clone->SetPosition(pos);
		clone->SetMinSize(minSize);
		clone->SetVisible(visible, false);
		widgets.push_back(clone);
	}
	void WidgetContainer::Insert(const std::shared_ptr<Widget> &widget)
	{
		assert(widget);
		widgets.push_back(widget);
	}
	void WidgetContainer::Insert(const std::shared_ptr<Widget> &widget, const math::Vec2 &pos)
	{
		assert(widget);
		widget->SetPosition(pos);
		widgets.push_back(widget);
	}
	void WidgetContainer::Insert(const std::shared_ptr<Widget> &widget, const math::Vec2 &pos, const math::Vec2 &minSize)
	{
		assert(widget);
		widget->SetPosition(pos);
		widget->SetMinSize(minSize);
		widgets.push_back(widget);
	}
	void WidgetContainer::Insert(const std::shared_ptr<Widget> &widget, const math::Vec2 &pos, const math::Vec2 &minSize, bool visible)
	{
		assert(widget);
		widget->SetPosition(pos);
		widget->SetMinSize(minSize);
		widget->SetVisible(visible, false);
		widgets.push_back(widget);
	}
}}
