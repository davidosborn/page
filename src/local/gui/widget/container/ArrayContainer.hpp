#ifndef    page_local_ui_widget_container_ArrayContainer_hpp
#   define page_local_ui_widget_container_ArrayContainer_hpp

#	include "Widget.hpp"
#	include "WidgetContainer.hpp"

namespace page { namespace gui
{
	class ArrayContainer :
		public Widget,
		public WidgetContainer
	{
		IMPLEMENT_CLONEABLE(ArrayContainer, Widget)

		/*-------------+
		| constructors |
		+-------------*/

		public:
		explicit ArrayContainer(bool horizontal, bool margin = true);

		// cursor event notification
		bool OnOver(const math::Vec2 &position, const res::Theme &, const math::Vec2 &size);
		bool OnDown(const math::Vec2 &position, const res::Theme &, const math::Vec2 &size);
		bool OnClick(const math::Vec2 &position, const res::Theme &, const math::Vec2 &size);

		private:
		// metrics
		WidgetSize CalcSize(const res::Theme &) const;

		// rendering
		void DoDraw(DrawContext &) const;

		// update
		void DoUpdate(float deltaTime);

		// child hit detection
		typedef std::function<bool (Widget *, const math::Vec2 &position, const math::Vec2 &size)> HitCallback;
		bool CheckHit(const math::Vec2 &position, const res::Theme &, const math::Vec2 &size, const HitCallback &);

		bool horizontal, margin;
	};
}}

#endif
