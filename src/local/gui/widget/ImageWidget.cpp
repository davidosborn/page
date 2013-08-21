#include <algorithm> // max, min

#include "../../math/interp.hpp" // HermiteScale
#include "../../res/type/Image.hpp" // Image::size
#include "../../res/type/Theme.hpp" // Theme::scale
#include "../../vid/DrawContext.hpp" // DrawContext::{GetFilterCaps,glowFilter,FilterSaver,PushGlowFilter}
#include "../DrawContext.hpp" // DrawContext::DrawImage
#include "ImageWidget.hpp"

namespace page { namespace ui
{
	/*--------------------------+
	| constructors & destructor |
	+--------------------------*/

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
			Select(size, Size::grow, Size::shrink));
	}

	void ImageWidget::DoDraw(DrawContext &context) const
	{
		context.DrawImage(*img, 0, 1);
	}
}}
