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

#include <algorithm> // max, min

#include "../../math/interp.hpp" // HermiteScale
#include "../../res/type/Image.hpp" // Image::size
#include "../../res/type/Theme.hpp" // Theme::scale
#include "../../vid/DrawContext.hpp" // DrawContext::{GetFilterCaps,glowFilter,FilterSaver,PushGlowFilter}
#include "../DrawContext.hpp" // DrawContext::DrawImage
#include "ImageWidget.hpp"

namespace page { namespace gui
{
	/*-------------+
	| constructors |
	+-------------*/

	ImageWidget::ImageWidget(
		cache::Proxy<res::Image> const& image,
		math::Vec2               const& size) :
			image(image),
			size(size) {}

	/*-----------------+
	| Widget overrides |
	+-----------------*/

	WidgetSize ImageWidget::CalcSize(const res::Theme &theme) const
	{
		return WidgetSize(
			Select(size, size * theme.scale, (*img)->size),
			Select(size, WidgetSize::Mode::grow, WidgetSize::Mode::shrink));
	}

	void ImageWidget::DoDraw(DrawContext &context) const
	{
		context.DrawImage(*img, 0, 1);
	}
}}
