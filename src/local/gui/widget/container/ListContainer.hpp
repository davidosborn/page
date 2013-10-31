#ifndef    page_local_ui_widget_container_ListContainer_hpp
#   define page_local_ui_widget_container_ListContainer_hpp

#	include <string>
#	include <vector>

#	include "../Widget.hpp"

namespace page { namespace gui
{
	class ListContainer : public Widget
	{
		IMPLEMENT_CLONEABLE(ListContainer, Widget)

		public:
		// construct/copy
		ListContainer(const math::Vec2 &size = 0);
		ListContainer(const ListContainer &);
		ListContainer &operator =(const ListContainer &);

		// modifiers
		void Insert(const std::string &);

		// cursor event notification
		bool OnOver(const math::Vec2 &position, const res::Theme &, const math::Vec2 &size);
		bool OnClick(const math::Vec2 &position, const res::Theme &, const math::Vec2 &size);

		private:
		// metrics
		WidgetSize CalcSize(const res::Theme &) const;

		// rendering
		void DoDraw(DrawContext &) const;

		math::Vec2 size;
		typedef std::vector<std::string> Items;
		Items items;
		Items::iterator selection;
	};
}}

#endif
