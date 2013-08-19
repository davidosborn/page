#ifndef    page_local_ui_widget_ButtonWidget_hpp
#   define page_local_ui_widget_ButtonWidget_hpp

#	include "Widget.hpp"

namespace page { namespace ui
{
	/**
	 * A button widget.
	 */
	class ButtonWidget :
		public Widget,
		public virtual util::Cloneable<ButtonWidget, Widget>
	{
		/*--------+
		| signals |
		+--------*/

		public:
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
	};
}}

#endif
