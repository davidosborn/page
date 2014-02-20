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

#include "../math/interp.hpp" // HermiteScale
#include "../vid/DrawContext.hpp" // DrawContext::{alphaFilter,GetFilterCaps,PushAlphaFilter}
#include "DrawContext.hpp" // DrawContext::GetBase
#include "Widget.hpp"

namespace page { namespace gui
{
	/*----------+
	| constants |
	+----------*/

	const float Widget::glowFadeInDuration        = 1.0 / 16;
	const float Widget::glowFadeOutDuration       = 0.5;
	const float Widget::glowFadeExponent          = 1.5;
	const float Widget::visibilityFadeInDuration  = 0.5;
	const float Widget::visibilityFadeOutDuration = 0.5;
	const float Widget::visibilityFadeExponent    = 1.5;

	/*-------------+
	| constructors |
	+-------------*/

	Widget::Widget(
		math::Vec2 const& position,
		math::Vec2 const& minSize,
		bool              visible) :
			position(position),
			minSize(minSize),
			visible(visible) {}

	/*------------+
	| positioning |
	+------------*/

	const math::Vec2 &Widget::GetPosition() const
	{
		return position;
	}

	const math::Vec2 &Widget::GetMinSize() const
	{
		return minSize;
	}

	WidgetSizeConstraints Widget::GetSizeConstraints(const res::Theme &theme) const
	{
		auto size(GetSizeConstraints(theme));
		size.min = Max(size.min, minSize * theme.scale);
		return size;
	}

	void Widget::SetPosition(const math::Vec2 &position)
	{
		this->position = position;
	}

	void Widget::SetMinSize(const math::Vec2 &minSize)
	{
		this->minSize = minSize;
	}

	/*-----------+
	| visibility |
	+-----------*/

	bool Widget::IsVisible() const
	{
		return visibility > 0 && opacity > 0;
	}

	void Widget::Show(bool fade)
	{
		SetVisible(true, fade);
	}

	void Widget::Hide(bool fade)
	{
		SetVisible(false, fade);
	}

	void Widget::SetVisible(bool visible, bool fade)
	{
		this->visible = visible;
		if (!fade) visibility = visible;
	}

	bool Widget::GetOpacity() const
	{
		return opacity;
	}

	void Widget::SetOpacity(float opacity)
	{
		this->opacity = opacity;
	}

	/*----------------+
	| special effects |
	+----------------*/

	bool Widget::IsGlowing() const
	{
		return glow;
	}

	void Widget::SetGlow(bool glow, bool fade)
	{
		this->glow = glow;
		if (!fade) glowiness = glow;
	}

	float Widget::GetGlowIntensity() const
	{
		return glowIntensity;
	}

	void Widget::SetGlowIntensity(float intensity)
	{
		glowIntensity = intensity;
	}

	/*---------------+
	| implementation |
	+---------------*/

	void Widget::Draw(DrawContext &context) const
	{
		if (!IsVisible()) return;

		DrawContext::Base::FilterSaver filterSaver(context.GetBase());

		// apply opacity filter
		if (visibility * opacity < 1 && (context.GetBase().GetFilterCaps() & DrawContext::Base::alphaFilter))
			context.GetBase().PushAlphaFilter(math::HermiteScale(visibility, visibilityFadeExponent) * opacity);

		// apply glow filter
		if (glowiness * glowIntensity > 0 && (context.GetBase().GetFilterCaps() & DrawContext::Base::glowFilter))
			context.GetBase().PushGlowFilter(math::HermiteScale(glowiness, glowFadeExponent) * glowIntensity);

		DoDraw(context);
	}

	void Widget::Update(float deltaTime)
	{
		DoUpdate(deltaTime);

		// update visibility
		visbility = visible ?
			std::min(visibility + deltaTime / visibilityFadeInDuration,  1.f) :
			std::max(visibility - deltaTime / visibilityFadeOutDuration, 0.f);

		// update glowiness
		glowiness = cursorOver ?
			std::min(glowiness + deltaTime / glowFadeInDuration,  1.f) :
			std::max(glowiness - deltaTime / glowFadeOutDuration, 0.f);
	}

	/*------------------+
	| virtual functions |
	+------------------*/

	void Widget::DoUpdate(float) {}
}}
