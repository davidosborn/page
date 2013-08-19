#ifndef    page_local_ui_widget_container_WidgetContainer_hpp
#   define page_local_ui_widget_container_WidgetContainer_hpp

#	include <memory> // shared_ptr
#	include <vector>

#	include "../math/fwd.hpp" // Vector

namespace page { namespace ui
{
	class Widget;

	struct WidgetContainer
	{
		// modifiers
		void Insert(const Widget &);
		void Insert(const Widget &, const math::Vec2 &position);
		void Insert(const Widget &, const math::Vec2 &position, const math::Vec2 &minSize);
		void Insert(const Widget &, const math::Vec2 &position, const math::Vec2 &minSize, bool visible);
		void Insert(const std::shared_ptr<Widget> &);
		void Insert(const std::shared_ptr<Widget> &, const math::Vec2 &position);
		void Insert(const std::shared_ptr<Widget> &, const math::Vec2 &position, const math::Vec2 &minSize);
		void Insert(const std::shared_ptr<Widget> &, const math::Vec2 &position, const math::Vec2 &minSize, bool visible);

		protected:
		typedef std::vector<std::shared_ptr<Widget>> Widgets;
		Widgets widgets;
	};
}}

#endif
