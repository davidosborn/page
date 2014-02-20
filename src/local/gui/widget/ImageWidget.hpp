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

#ifndef    page_local_ui_widget_ImageWidget_hpp
#   define page_local_ui_widget_ImageWidget_hpp

#	include "../../cache/proxy/Proxy.hpp"
#	include "Widget.hpp"

namespace page { namespace res { class Image; }}

namespace page { namespace gui
{
	/**
	 * A widget that displays an image.
	 */
	class ImageWidget : public Widget
	{
		IMPLEMENT_CLONEABLE(ImageWidget, Widget)

		/*-------------+
		| constructors |
		+-------------*/

		public:
		explicit ImageWidget(
			cache::Proxy<res::Image> const& image,
			math::Vec2               const& size = 0);

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
		 * The widget's image content.
		 */
		cache::Proxy<res::Image> image;

		/**
		 * The widget's size.
		 */
		math::Vec2 size;
	};
}}

#endif
