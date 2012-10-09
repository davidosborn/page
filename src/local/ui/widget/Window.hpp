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

#ifndef    page_ui_widget_Window_hpp
#   define page_ui_widget_Window_hpp

#	include <memory> // shared_ptr
#	include <string>
#	include "../Widget.hpp"

namespace page
{
	namespace ui
	{
		struct Window : Widget
		{
			// construct
			explicit Window(const std::string &title = "");
			Window(const std::string &title, const Widget &child);
			Window(const std::string &title, const std::shared_ptr<Widget> &child);

			// clone
			Window *Clone() const;

			// cursor event notification
			bool OnOver(const math::Vector<2> &position, const res::Theme &, const math::Vector<2> &size);
			bool OnDown(const math::Vector<2> &position, const res::Theme &, const math::Vector<2> &size);
			bool OnClick(const math::Vector<2> &position, const res::Theme &, const math::Vector<2> &size);

			private:
			// metrics
			Size CalcSize(const res::Theme &) const;

			// rendering
			void DoDraw(DrawContext &) const;

			// update
			void DoUpdate(float deltaTime);

			// internal metrics
			math::Vector<2> GetTitleSize(const res::Theme &) const;
			float GetTitleHeight(const res::Theme &) const;

			// child hit detection
			typedef std::function<bool (Widget *, const math::Vector<2> &position, const math::Vector<2> &size)> HitCallback;
			bool CheckHit(const math::Vector<2> &position, const res::Theme &, const math::Vector<2> &size, const HitCallback &);

			std::string title;
			std::shared_ptr<Widget> child;
		};
	}
}

#endif
