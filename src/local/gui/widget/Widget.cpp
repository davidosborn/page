#include <algorithm> // max, min

#include "../math/interp.hpp" // HermiteScale
#include "../vid/DrawContext.hpp" // DrawContext::{alphaFilter,GetFilterCaps,PushAlphaFilter}
#include "DrawContext.hpp" // DrawContext::GetBase
#include "Widget.hpp"

namespace page { namespace ui
{
	/*----------+
	| constants |
	+----------*/

	constexpr float Widget::glowFadeInDuration     = 1.0 / 16;
	constexpr float Widget::glowFadeOutDuration    = 0.5;
	constexpr float Widget::glowFadeExponent       = 1.5;
	constexpr float Widget::visibilityFadeInDuration  = 0.5;
	constexpr float Widget::visibilityFadeOutDuration = 0.5;
	constexpr float Widget::visibilityFadeExponent    = 1.5;

////////// WidgetSize //////////////////////////////////////////////////////////

	/*--------------------------+
	| constructors & destructor |
	+--------------------------*/

	WidgetSize::WidgetSize(
		math::Vec2            const& min,
		math::Vector<2, Mode> const& mode) :
			min(min), mode(mode) {}

////////// Widget //////////////////////////////////////////////////////////////

	/*--------------------------+
	| constructors & destructor |
	+--------------------------*/

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

	WidgetSize Widget::GetSize(const res::Theme &theme) const
	{
		Size size(CalcSize(theme));
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
