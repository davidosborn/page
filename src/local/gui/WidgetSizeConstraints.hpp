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
