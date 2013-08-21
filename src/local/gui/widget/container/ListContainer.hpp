#ifndef    page_local_ui_widget_container_List_hpp
#   define page_local_ui_widget_container_List_hpp

#	include <string>
#	include <vector>

#	include "../Widget.hpp"

namespace page { namespace ui
{
	class List :
		public Widget,
		public virtual util::Cloneable<List, Widget>
	{
		public:
		// construct/copy
		List(const math::Vec2 &size = 0);
		List(const List &);
		List &operator =(const List &);

		// modifiers
		void Insert(const std::string &);

		// cursor event notification
		bool OnOver(const math::Vec2 &position, const res::Theme &, const math::Vec2 &size);
		bool OnClick(const math::Vec2 &position, const res::Theme &, const math::Vec2 &size);

		private:
		// metrics
		Size CalcSize(const res::Theme &) const;

		// rendering
		void DoDraw(DrawContext &) const;

		math::Vec2 size;
		typedef std::vector<std::string> Items;
		Items items;
		Items::iterator selection;
	};
}}

#endif
