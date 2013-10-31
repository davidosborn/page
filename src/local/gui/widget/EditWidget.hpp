#ifndef    page_local_ui_widget_EditWidget_hpp
#   define page_local_ui_widget_EditWidget_hpp

#	include <string>

#	include "Widget.hpp"

namespace page { namespace gui
{
	/**
	 * An edit-box widget.
	 */
	class EditWidget : public Widget
	{
		IMPLEMENT_CLONEABLE(EditWidget, Widget)

		/*--------+
		| signals |
		+--------*/

		public:
		/**
		 * A signal that is called when the user types while the widget has
		 * keyboard focus or otherwise changes the widget's text.
		 */
		util::copyable_signal<void ()> onChange;

		/**
		 * A signal that is called when the user types enter/return while the
		 * widget has keyboard focus.
		 */
		util::copyable_signal<void ()> onSubmit;

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
		 * The widget's text.
		 */
		std::string text;
	};
}}

#endif
