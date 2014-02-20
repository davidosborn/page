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

#ifndef    page_local_gui_Root_hpp
#   define page_local_gui_Root_hpp

#	include "../math/Aabb.hpp"
#	include "../util/class/special_member_functions.hpp" // Polymorphic
#	include "widget/container/WidgetContainer.hpp"

namespace page
{
	namespace res { class Theme; }
	namespace vid { class DrawContext; }
}

namespace page { namespace gui
{
	class Widget;

	/**
	 * The root node of the GUI hierarchy.
	 */
	class Root :
		public WidgetContainer,
		public util::Polymorphic<Root>
	{
		/*-------------+
		| constructors |
		+-------------*/

		public:
		explicit Root(const std::shared_ptr<const res::Theme> &);

		/*------+
		| theme |
		+------*/

		/**
		 * @return The GUI's theme.
		 */
		const res::Theme &GetTheme() const;

		/**
		 * Sets the GUI's theme.
		 */
		void SetTheme(const std::shared_ptr<const res::Theme> &);

		/*--------+
		| metrics |
		+--------*/

		/**
		 *
		 */
		math::Aabb<2> GetBounds(const Widget &, const vid::DrawContext &) const;

		/**
		 *
		 */
		math::Aabb<2> GetBounds(const Widget &, float aspect) const;

		// rendering
		void Draw(vid::DrawContext &) const;

		// update
		void Update(float deltaTime);

		/**
		 * Updates the mouse-over state of child widgets.
		 */
		bool UpdateCursor(const math::Vec2 &cursorPosition, float aspect);

		/*-------------+
		| data members |
		+-------------*/

		private:
		/**
		 * The GUI's theme.
		 */
		std::shared_ptr<const res::Theme> theme;
	};
}}

#endif
