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

#ifndef    page_local_ui_DrawContext_hpp
#   define page_local_ui_DrawContext_hpp

#	include "../res/type/Font.hpp" // TextAlign
#	include "../res/type/Theme.hpp"

namespace page
{
	namespace vid { class DrawContext; }

	namespace ui
	{
		struct DrawContext
		{
			typedef vid::DrawContext Base;

			// construct
			DrawContext(Base &, const res::Theme &);

			// base context access
			Base &GetBase();
			const Base &GetBase() const;

			// theme access
			const res::Theme &GetTheme() const;

			// metrics
			math::Vector<2> GetScale() const;

			// image rendering
			void DrawBackground(
				const res::Theme::Background &,
				const math::Aabb<2> & = math::Aabb<2>(0, 1));
			void DrawFrame(
				const res::Theme::Frame &,
				const math::Aabb<2> & = math::Aabb<2>(0, 1));
			void DrawDecorations(
				const res::Theme::Decorations &,
				const math::Aabb<2> & = math::Aabb<2>(0, 1),
				bool background = false);
			void DrawImage(
				const cache::Proxy<res::Image> &,
				const math::Vector<2> &position = 0,
				const math::Vector<2> &size = 0,
				const math::Vector<2, bool> &tile = false,
				const math::Vector<2, bool> &flip = false,
				const math::Vector<2> &center = 0, float angle = 0);

			// text rendering
			void DrawText(
				const res::Theme::Text &, const std::string &text,
				const math::Aabb<2> & = math::Aabb<2>(0, 1),
				const math::RgbaColor<> &color = math::WhiteRgbaColor(),
				bool wrap = true, res::TextAlign = res::leftTextAlign);

			private:
			Base &base;
			const res::Theme &theme;
			float scale;
		};
	}
}

#endif
