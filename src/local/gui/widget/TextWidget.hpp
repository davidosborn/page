#ifndef    page_local_ui_widget_TextWidget_hpp
#   define page_local_ui_widget_TextWidget_hpp

#	include <string>

#	include "../../math/Color.hpp" // RgbaColor
#	include "Widget.hpp"

namespace page { namespace ui
{
	/**
	 * A widget that displays a block of text.
	 */
	class TextWidget :
		public Widget,
		public virtual util::Cloneable<TextWidget, Widget>
	{
		/*--------------------------+
		| constructors & destructor |
		+--------------------------*/

		public:
		explicit TextWidget(
			std::string       const& text  = "",
			math::Vec2        const& size  = 0,
			math::RgbaColor<> const& color = math::WhiteRgbaColor(),
			bool                     wrap  = true);

		/*-----------+
		| properties |
		+-----------*/

		/**
		 * Returns the widget's text.
		 */
		const std::string &GetText() const;

		/**
		 * Sets the widget's text.
		 */
		void SetText(const std::string &);

		/*--------+
		| signals |
		+--------*/

		/**
		 * A signal that is called when the user clicks on the widget.
		 */
		util::copyable_signal<void ()> onClick;

		/*-----------------+
		| Widget overrides |
		+-----------------*/

		private:
		WidgetSize CalcSize(const res::Theme &) const override;
		void DoDraw(DrawContext &) const override;

		/*-------------+
		| data members |
		+-------------*/

		/**
		 * The widget's text content.
		 */
		std::string text;

		/**
		 * The widget's size.
		 */
		math::Vec2 size;

		/**
		 * The color of the text.
		 */
		math::RgbaColor<> color;

		/**
		 * @c true if the text should wrap.
		 */
		bool wrap;
	};
}}

#endif
