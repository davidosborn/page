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

#ifndef    page_local_ui_widget_container_Window_hpp
#   define page_local_ui_widget_container_Window_hpp

#	include <string>

#	include "../Widget.hpp"
#	include "WidgetContainer.hpp"

namespace page { namespace gui
{
	/**
	 * A window widget.
	 */
	class Window :
		public Widget,
		public WidgetContainer
	{
		IMPLEMENT_CLONEABLE(Window, Widget)

		/*-------------+
		| constructors |
		+-------------*/

		public:
		explicit Window(const std::string &title = "");
		Window(const std::string &title, const Widget &content);
		Window(const std::string &title, const std::shared_ptr<Widget> &content);

		/*-----------+
		| properties |
		+-----------*/

		/**
		 * @return @c title.
		 */
		const std::string &GetTitle() const;

		/**
		 * @return @c content.
		 */
		const Widget &GetContent() const;

		/**
		 * Sets @c title.
		 */
		void SetTitle(const std::string &);

		/**
		 * Sets @c content.
		 */
		void SetContent(const Widget &);

		/**
		 * Sets @c content.
		 */
		void SetContent(Widget &&);

		/*-----------------+
		| Widget overrides |
		+-----------------*/

		private:
		WidgetSize CalcSize(const res::Theme &) const override;
		void DoDraw(DrawContext &) const override;
		void DoUpdate(float deltaTime) override;

		/*----------------+
		| signal handlers |
		+----------------*/

		void OnClick(unsigned button);
		void OnCursorEnter();
		void OnCursorLeave();

		/*-----------------+
		| internal metrics |
		+-----------------*/

		math::Vec2 GetTitleSize(const res::Theme &) const;
		float GetTitleHeight(const res::Theme &) const;

		/*--------------------+
		| child hit detection |
		+--------------------*/

		typedef std::function<bool (Widget *, const math::Vec2 &position, const math::Vec2 &size)> HitCallback;
		bool CheckHit(const math::Vec2 &position, const res::Theme &, const math::Vec2 &size, const HitCallback &);

		/*-------------+
		| data members |
		+-------------*/

		/**
		 * The window's title.
		 */
		std::string title;

		/**
		 * The window's content.
		 */
		std::unique_ptr<Widget> content;

		boost::signals::scoped_connection onClickConnection;
		boost::signals::scoped_connection onCursorEnterConnection;
		boost::signals::scoped_connection onCursorLeaveConnection;
	};
}}

#endif
