/**
 * @section license
 *
 * Copyright (c) 2006-2012 David Osborn
 *
 * Permission is granted to use and redistribute this software in source and
 * binary form, with or without modification, subject to the following
 * conditions:
 *
 * 1. Redistributions in source form must retain the above copyright notice,
 *    this list of conditions, and the following disclaimer.
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

#include "../cache/Proxy.hpp"
#include "../math/float.hpp" // DegToRad
#include "../res/type/Image.hpp" // Image::size
#include "../vid/DrawContext.hpp" // DrawContext::{Draw{Quad,Text},Get{Frame,PixelLogicalBox}}
#include "DrawContext.hpp"

namespace page
{
	namespace ui
	{
		// construct
		DrawContext::DrawContext(Base &base, const res::Theme &theme) :
			base(base), theme(theme), scale(theme.scale ?
				(Size(base.GetFrame()) * Size(base.GetPixelLogicalBox())).y / theme.scale : 1) {}

		// base context access
		DrawContext::Base &DrawContext::GetBase()
		{
			return base;
		}
		const DrawContext::Base &DrawContext::GetBase() const
		{
			return base;
		}

		// theme access
		const res::Theme &DrawContext::GetTheme() const
		{
			return theme;
		}

		// metrics
		math::Vector<2> DrawContext::GetScale() const
		{
			return scale / (Size(base.GetFrame()) * Size(base.GetPixelLogicalBox()));
		}

		// specialized image rendering
		void DrawContext::DrawBackground(
			const res::Theme::Background &background, const math::Aabb<2> &aabb)
		{
			if (background.image)
				// FIXME: support res::Theme::Background::{offset,scale}
				DrawImage(background.image, aabb.min, Size(aabb), true, background.flip);
		}
		void DrawContext::DrawFrame(const res::Theme::Frame &frame,
			const math::Aabb<2> &aabb)
		{
			// FIXME: support res::Theme::Frame::{Corner,Side}::flip
			// FIXME: draw corners
			// FIXME: when frame meets corner, make edge flat against corner
			// FIXME: when frame does not meet other frame or corner, make edge
			// flat against center of corner position
			math::Vector<2> scale(GetScale());
			if (frame.left.image)
			{
				math::Vector<2>
					size(frame.left.image->size),
					thickness(size.y * scale);
				float
					repeat(1 / (size.x * scale.y)),
					inset(thickness.y * repeat);
				base.DrawQuad(frame.left.image,
					aabb.min, 0,
					math::Vector<2>(aabb.min.x, aabb.max.y), math::Vector<2>(repeat, 0),
					math::Vector<2>(aabb.min.x + thickness.x, aabb.max.y - thickness.y), math::Vector<2>(repeat - inset, 1),
					aabb.min + thickness, math::Vector<2>(inset, 1),
					math::Vector<2, bool>(false, true));
			}
			if (frame.top.image)
			{
				math::Vector<2>
					size(frame.top.image->size),
					thickness(size.y * scale);
				float
					repeat(1 / (size.x * scale.x)),
					inset(thickness.x * repeat);
				base.DrawQuad(frame.top.image,
					aabb.min, 0,
					aabb.min + thickness, math::Vector<2>(inset, 1),
					math::Vector<2>(aabb.max.x - thickness.x, aabb.min.y + thickness.y), math::Vector<2>(repeat - inset, 1),
					math::Vector<2>(aabb.max.x, aabb.min.y), math::Vector<2>(repeat, 0),
					math::Vector<2, bool>(false, true));
			}
			if (frame.right.image)
			{
				math::Vector<2>
					size(frame.right.image->size),
					thickness(size.y * scale);
				float
					repeat(1 / (size.x * scale.y)),
					inset(thickness.y * repeat);
				base.DrawQuad(frame.right.image,
					math::Vector<2>(aabb.max.x - thickness.x, aabb.min.y + thickness.y), math::Vector<2>(inset, 0),
					math::Vector<2>(aabb.max - thickness), math::Vector<2>(repeat - inset, 0),
					aabb.max, math::Vector<2>(repeat, 1),
					math::Vector<2>(aabb.max.x, aabb.min.y), math::Vector<2>(0, 1),
					math::Vector<2, bool>(false, true));
			}
			if (frame.bottom.image)
			{
				math::Vector<2>
					size(frame.bottom.image->size),
					thickness(size.y * scale);
				float
					repeat(1 / (size.x * scale.x)),
					inset(thickness.x * repeat);
				base.DrawQuad(frame.bottom.image,
					math::Vector<2>(aabb.min.x + thickness.x, aabb.max.y - thickness.y), math::Vector<2>(inset, 0),
					math::Vector<2>(aabb.min.x, aabb.max.y), math::Vector<2>(0, 1),
					aabb.max, math::Vector<2>(repeat, 1),
					aabb.max - thickness, math::Vector<2>(repeat - inset, 0),
					math::Vector<2, bool>(false, true));
			}
		}
		void DrawContext::DrawDecorations(
			const res::Theme::Decorations &decorations,
			const math::Aabb<2> &aabb, bool background)
		{
			for (res::Theme::Decorations::const_iterator decoration(decorations.begin()); decoration != decorations.end(); ++decoration)
				// FIXME: support res::Theme::Decoration::offset
				if (decoration->background == background)
					DrawImage(decoration->image,
					aabb.min + decoration->position * Size(aabb),
					0, false, decoration->flip, decoration->center,
					math::DegToRad(decoration->angle));
		}
		void DrawContext::DrawImage(const cache::Proxy<res::Image> &img,
			const math::Vector<2> &pos, const math::Vector<2> &size,
			const math::Vector<2, bool> &tile,
			const math::Vector<2, bool> &flip, const math::Vector<2> &center,
			float angle)
		{
			math::Vector<2>
				scale(GetScale()),
				imgSize(img->size * scale),
				drawSize(Select(size, size, imgSize)),
				centerPos(pos - drawSize * center),
				repeat(Select(tile, drawSize / imgSize, 1));
			// calculate vertex coordinates
			math::Vector<2> co[] =
			{
				centerPos,
				math::Vector<2>(centerPos.x, centerPos.y + drawSize.y),
				centerPos + drawSize,
				math::Vector<2>(centerPos.x + drawSize.x, centerPos.y)
			};
			if (flip.x)
			{
				float offset = (center.x * 2 - 1) * drawSize.x;
				for (unsigned i = 0; i < 4; ++i) co[i].x += offset;
			}
			if (flip.y)
			{
				float offset = (center.y * 2 - 1) * drawSize.y;
				for (unsigned i = 0; i < 4; ++i) co[i].y += offset;
			}
			if (angle)
				for (unsigned i = 0; i < 4; ++i)
					co[i] = Rotate((co[i] - pos) / scale, angle) * scale + pos;
			// calculate texture coordinates
			math::Vector<2> uv[] =
			{
				0,      math::Vector<2>(0, repeat.y),
				repeat, math::Vector<2>(repeat.x, 0)
			};
			if (flip.x)
			{
				swap(uv[0], uv[3]);
				swap(uv[1], uv[2]);
			}
			if (flip.y)
			{
				swap(uv[0], uv[1]);
				swap(uv[2], uv[3]);
			}
			base.DrawQuad(img,
				co[0], uv[0],
				co[1], uv[1],
				co[2], uv[2],
				co[3], uv[3], !tile || !size);
		}

		// specialized text rendering
		void DrawContext::DrawText(const res::Theme::Text &style,
			const std::string &text, const math::Aabb<2> &aabb,
			const math::RgbaColor<> &color, bool wrap, res::TextAlign align)
		{
			math::HsvColor<> bottomColor(Complement(math::HsvColor<>(color)));
			bottomColor *= math::HsvColor<>(1, .5, .65);
			base.DrawText(text, style.font, style.size * GetScale().y,
				aabb, wrap, align, color,
				math::RgbaColor<>(bottomColor, color.a), .1,
				math::BlackRgbaColor());
		}
	}
}
