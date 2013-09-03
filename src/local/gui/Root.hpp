#ifndef    page_local_gui_Root_hpp
#   define page_local_gui_Root_hpp

#	include "../math/Aabb.hpp"
#	include "../util/class/special_member_functions.hpp" // Polymorphic
#	include "WidgetContainer.hpp"

namespace page
{
	namespace res { class Theme; }
	namespace vid { class DrawContext; }
}

namespace page { namespace gui
{
	class Widget;

	/**
	 * Represents the GUI.
	 */
	class Root :
		public WidgetContainer,
		public util::Polymorphic<Root>
	{
		/*-------------+
		| constructors |
		+-------------*/

		explicit Root(const std::shared_ptr<const res::Theme> &);

		/*-----------+
		| properties |
		+-----------*/

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
		bool UpdateCursor(const math::Vec2 &position, float aspect);

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
