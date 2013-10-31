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
