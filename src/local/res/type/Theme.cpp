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

#include "Image.hpp" // Image::size
#include "register.hpp" // REGISTER_TYPE
#include "Theme.hpp"

namespace page
{
	namespace res
	{
		// construct
		Theme::Theme() : margin(16), scale(1024) {}
		Theme::Background::Background() : scale(true) {}
		Theme::Decoration::Decoration() : background(false), center(.5), angle(0) {}
		Theme::Panel::Panel() : margin(0) {}
		Theme::Text::Text() : size(0) {}

		// metrics
		math::Aabb<2> GetThickness(const Theme::Frame &frame)
		{
			return math::Aabb<2>(
				math::Vector<2>(
					frame.left.image   ? frame.left.image->size.y   : 0,
					frame.top.image    ? frame.top.image->size.y    : 0),
				math::Vector<2>(
					frame.right.image  ? frame.right.image->size.y  : 0,
					frame.bottom.image ? frame.bottom.image->size.y : 0));
		}

		// defaults
		void SetDefaults(Theme &theme)
		{
			// FIXME: panel margin of zero is defaulting to theme margin,
			// but we may need a panel margin of zero at some point
			if (!theme.text.font)
				theme.text.font = cache::Resource<Font>("font/default.ttf");
			if (!theme.text.size)
				theme.text.size = 16;
			if (!theme.window.margin)
				theme.window.margin = theme.margin;
			if (!theme.window.title.text.font)
				theme.window.title.text.font = theme.text.font;
			if (!theme.window.title.text.size)
				theme.window.title.text.size = theme.text.size;
			if (!theme.button.margin)
				theme.button.margin = theme.margin;
			if (!theme.button.text.font)
				theme.button.text.font = theme.text.font;
			if (!theme.button.text.size)
				theme.button.text.size = theme.text.size;
			if (!theme.edit.margin)
				theme.edit.margin = theme.margin;
			if (!theme.edit.text.font)
				theme.edit.text.font = theme.text.font;
			if (!theme.edit.text.size)
				theme.edit.text.size = theme.text.size;
			if (!theme.list.margin)
				theme.list.margin = theme.margin;
			if (!theme.list.text.font)
				theme.list.text.font = theme.text.font;
			if (!theme.list.text.size)
				theme.list.text.size = theme.text.size;
		}

		void PostLoadTheme(Theme &theme)
		{
			SetDefaults(theme);
		}

		REGISTER_TYPE(Theme, "theme", PostLoadTheme)
	}
}
