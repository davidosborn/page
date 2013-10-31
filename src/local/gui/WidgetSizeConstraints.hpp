#ifndef    page_local_gui_WidgetSizeConstraints_hpp
#   define page_local_gui_WidgetSizeConstraints_hpp

namespace page { namespace gui
{
	/**
	 * A set of constraints that determine the size of a widget.
	 */
	struct WidgetSizeConstraints
	{
		/**
		 * The widget's fill mode, which determines how the widget should fit
		 * within its container, relative to its neighbouring widgets.
		 */
		enum class FillMode
		{
			/**
			 * The widget will shrink to its minimum size.
			 */
			shrinkWrap,

			/**
			 * The widget will grow to fill the space of its container.
			 */
			fill,

			/**
			 * The widget will grow from its minimum size to the size of its
			 * neighbouring widgets.
			 */
			growToFit
		};

		/**
		 * The widget's minimum size.
		 */
		math::Vec2 minSize;

		/**
		 * The widget's fill mode.
		 */
		math::Vector<2, FillMode> fillMode = FillMode::shrinkWrap;
	};
}}

#endif
