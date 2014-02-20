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
