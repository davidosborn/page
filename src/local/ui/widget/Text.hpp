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

#ifndef    page_local_ui_widget_Text_hpp
#   define page_local_ui_widget_Text_hpp

#	include <string>
#	include "../../math/Color.hpp" // RgbaColor
#	include "../../util/copy_ptr.hpp"
#	include "../../util/Signal.hpp"
#	include "../Widget.hpp"

namespace page
{
	namespace ui
	{
		struct Text : Widget
		{
			Text(const std::string &text = "", const math::Vector<2> &size = 0,
				const math::RgbaColor<> & = math::WhiteRgbaColor(),
				bool wrap = true);

			// clone
			Text *Clone() const;

			// modifiers
			void SetText(const std::string &);

			// signals
			util::Signal<void ()> clickSig;

			private:
			// metrics
			Size CalcSize(const res::Theme &) const;

			// rendering
			void DoDraw(DrawContext &) const;

			std::string text;
			math::Vector<2> size;
			math::RgbaColor<> color;
			bool wrap;
		};
	}
}

#endif
