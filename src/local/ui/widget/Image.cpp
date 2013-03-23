/**
 * @section license
 *
 * Copyright (c) 2006-2013 David Osborn
 *
 * Permission is granted to use and redistribute this software in source and
 * binary form, with or without modification, subject to the following
 * conditions:
 *
 * 1. Redistributions in source form must retain the above copyright notice,
 *    this list of conditions, and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions, and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution, and in the same
 *    place and form as other copyright, license, and disclaimer information.
 *
 * As a special exception, distributions of derivative works in binary form may
 * include an acknowledgement in place of the above copyright notice, this list
 * of conditions, and the following disclaimer in the documentation and/or other
 * materials provided with the distribution, and in the same place and form as
 * other acknowledgements, similar in substance to the following:
 *
 *    Portions of this software are based on the work of David Osborn.
 *
 * This software is provided "as is", without any express or implied warranty.
 * In no event will the authors be liable for any damages arising out of the use
 * of this software.
 */

#include <algorithm> // max, min

#include "../../cache/proxy/Proxy.hpp"
#include "../../math/interp.hpp" // HermiteScale
#include "../../res/type/Image.hpp" // Image::size
#include "../../res/type/Theme.hpp" // Theme::scale
#include "../../vid/DrawContext.hpp" // DrawContext::{GetFilterCaps,glowFilter,FilterSaver,PushGlowFilter}
#include "../DrawContext.hpp" // DrawContext::DrawImage
#include "Image.hpp"

namespace page { namespace ui
{
	// construct
	Image::Image(const cache::Proxy<res::Image> &img, const math::Vector<2> &size) :
		img(img.Copy()), size(size), cursorOver(false), glowIntensity(0) {}

	// cursor event notification
	bool Image::OnOver(const math::Vector<2> &, const res::Theme &, const math::Vector<2> &)
	{
		cursorOver = true;
	}

	// metrics
	Image::Size Image::CalcSize(const res::Theme &theme) const
	{
		return Size(
			Select(size, size * theme.scale, (*img)->size),
			Select(size, Size::grow, Size::shrink));
	}

	// rendering
	void Image::DoDraw(DrawContext &context) const
	{
		DrawContext::Base::FilterSaver filterSaver(context.GetBase());
		if (glowIntensity && context.GetBase().GetFilterCaps() & DrawContext::Base::glowFilter)
			context.GetBase().PushGlowFilter(math::HermiteScale(glowIntensity, glowFadeExponent));
		context.DrawImage(*img, 0, 1);
	}

	// update
	void Image::DoUpdate(float deltaTime)
	{
		// update glow intensity
		glowIntensity = cursorOver ?
			std::min(glowIntensity + deltaTime / glowFadeInDuration,  1.f) :
			std::max(glowIntensity - deltaTime / glowFadeOutDuration, 0.f);
		// reset cursor over state
		cursorOver = false;
	}
}}
