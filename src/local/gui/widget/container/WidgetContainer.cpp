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
