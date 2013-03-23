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

#include <algorithm> // count, max

#include "../../res/type/Font.hpp" // Font::{lineHeight,maxBearing}, GetMaxLineWidth
#include "../../res/type/Theme.hpp" // GetThickness, Theme::window
#include "../../vid/DrawContext.hpp" // DrawContext::{FrameSaver,PushFrame}
#include "../DrawContext.hpp"
#include "Window.hpp"

namespace page { namespace ui
{
	// construct
	Window::Window(const std::string &title) : title(title) {}
	Window::Window(const std::string &title, const Widget &child) :
		title(title), child(child.Clone()) {}
	Window::Window(const std::string &title, const std::shared_ptr<Widget> &child) :
		title(title), child(child) {}

	// cursor event notification
	bool Window::OnOver(const math::Vector<2> &pos, const res::Theme &theme, const math::Vector<2> &size)
	{
		using namespace std::placeholders;
		return CheckHit(pos, theme, size,
			std::bind(&Widget::OnOver, _1, _2, theme, _3));
	}
	bool Window::OnDown(const math::Vector<2> &pos, const res::Theme &theme, const math::Vector<2> &size)
	{
		using namespace std::placeholders;
		return CheckHit(pos, theme, size,
			std::bind(&Widget::OnDown, _1, _2, theme, _3));
	}
	bool Window::OnClick(const math::Vector<2> &pos, const res::Theme &theme, const math::Vector<2> &size)
	{
		using namespace std::placeholders;
		return CheckHit(pos, theme, size,
			std::bind(&Widget::OnClick, _1, _2, theme, _3));
	}

	// metrics
	Window::Size Window::CalcSize(const res::Theme &theme) const
	{
		// account for frame and margin
		math::Aabb<2> frameThickness(GetThickness(theme.window.frame));
		Size size(frameThickness.min + frameThickness.max + theme.window.margin * 2);
		// account for title
		math::Vector<2> titleSize(GetTitleSize(theme));
		size.min.y += titleSize.y;
		// account for child widget
		if (child)
		{
			Size childSize(child->GetSize(theme));
			size.min.x += std::max(childSize.min.x, titleSize.x);
			size.min.y += childSize.min.y;
			size.mode = Max(childSize.mode, size.mode);
		}
		else size.min.x = titleSize.x;
		return size;
	}

	// rendering
	void Window::DoDraw(DrawContext &context) const
	{
		// calculate corner coordinates
		const res::Theme &theme(context.GetTheme());
		math::Vector<2> scale(context.GetScale());
		math::Aabb<2>
			frameThickness(GetThickness(theme.window.frame) * scale),
			cornerBox(Shrink(math::Aabb<2>(0, 1),
				frameThickness.min / 2,
				frameThickness.max / 2));
		// draw background
		context.DrawBackground(theme.window.background, cornerBox);
		// draw background decorations
		context.DrawDecorations(theme.window.decorations, cornerBox, true);
		// draw title
		math::Aabb<2> childBox(Shrink(math::Aabb<2>(0, 1),
			frameThickness.min + theme.window.margin * scale,
			frameThickness.max + theme.window.margin * scale));
		if (!title.empty())
		{
			const res::Font &font(*theme.window.title.text.font);
			float
				lineHeight = font.lineHeight * theme.window.title.text.size * scale.y,
				maxBearing = font.maxBearing.y * theme.window.title.text.size * scale.y,
				halfLineGap = (lineHeight - maxBearing) / 2;
			math::Aabb<2> titleBox(childBox.min, math::Vector<2>(
				childBox.max.x,
				childBox.min.y +
					(std::count(title.begin(), title.end(), '\n') + 1) *
					lineHeight + halfLineGap));
			context.DrawBackground(theme.window.title.background, titleBox);
			titleBox.min.y += halfLineGap;
			context.DrawText(theme.window.title.text, title, titleBox,
				math::WhiteRgbaColor(), false, res::centerTextAlign);
			childBox.min.y = titleBox.max.y + theme.window.margin * scale.y;
		}
		// draw child widget
		if (child)
		{
			// FIXME: handle the case where the title is wider than the
			// child widget, but the child widget doesn't want to expand
			// to fill the extra space
			DrawContext::Base::FrameSaver frameSaver(context.GetBase());
			context.GetBase().PushFrame(childBox);
			child->Draw(context);
		}
		// draw frame
		context.DrawFrame(theme.window.frame);
		// draw foreground decorations
		context.DrawDecorations(theme.window.decorations, cornerBox);
	}

	// update
	void Window::DoUpdate(float deltaTime)
	{
		if (child) child->Update(deltaTime);
	}

	// internal metrics
	math::Vector<2> Window::GetTitleSize(const res::Theme &theme) const
	{
		if (title.empty()) return 0;
		return math::Vector<2>(
			GetMaxLineWidth(*theme.window.title.text.font, title.begin(),
				title.end()) * theme.window.title.text.size,
			GetTitleHeight(theme));
	}
	float Window::GetTitleHeight(const res::Theme &theme) const
	{
		if (title.empty()) return 0;
		const res::Font &font(*theme.window.title.text.font);
		float
			lineHeight = font.lineHeight * theme.window.title.text.size,
			maxBearing = font.maxBearing.y * theme.window.title.text.size,
			halfLineGap = (lineHeight - maxBearing) / 2;
		return halfLineGap + theme.window.margin +
			(std::count(title.begin(), title.end(), '\n') + 1) * lineHeight;
	}

	// child hit detection
	bool Window::CheckHit(const math::Vector<2> &pos, const res::Theme &theme, const math::Vector<2> &size, const HitCallback &cb)
	{
		bool result = false;
		if (child)
		{
			// account for frame and margin
			math::Aabb<2>
				frameThickness(GetThickness(theme.window.frame)),
				aabb(Shrink(math::Aabb<2>(0, size),
					frameThickness.min + theme.window.margin,
					frameThickness.max + theme.window.margin));
			// account for title
			aabb.min.y += GetTitleHeight(theme);
			// account for child widget
			Size childSize(child->GetSize(theme));
			float width = aabb.max.x - aabb.min.x;
			if (width > childSize.min.x && childSize.mode.y == Size::shrink)
				aabb = Shrink(aabb, math::Vector<2>(width - childSize.min.x, 0));
			// test intersection
			if (Contains(aabb, pos))
				result |= cb(child.get(), pos - aabb.min, math::Size(aabb));
		}
		return result;
	}
}}
