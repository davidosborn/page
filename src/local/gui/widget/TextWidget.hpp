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

#ifndef    page_local_ui_widget_TextWidget_hpp
#   define page_local_ui_widget_TextWidget_hpp

#	include <string>

#	include "../../math/Color.hpp" // RgbaColor
#	include "Widget.hpp"

namespace page { namespace gui
{
	/**
	 * A widget that displays a block of text.
	 */
	class TextWidget : public Widget
	{
		IMPLEMENT_CLONEABLE(TextWidget, Widget)

		/*-------------+
		| constructors |
		+-------------*/

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
