#ifndef    page_local_ui_widget_container_Window_hpp
#   define page_local_ui_widget_container_Window_hpp

#	include <string>

#	include "../Widget.hpp"
#	include "WidgetContainer.hpp"

namespace page { namespace ui
{
	/**
	 * A window widget.
	 */
	class Window :
		public Widget,
		public WidgetContainer,
		public virtual util::Cloneable<Window, Widget>
	{
		/*--------------------------+
		| constructors & destructor |
		+--------------------------*/

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
		Size CalcSize(const res::Theme &) const override;
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
