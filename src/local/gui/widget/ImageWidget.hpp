#ifndef    page_local_ui_widget_ImageWidget_hpp
#   define page_local_ui_widget_ImageWidget_hpp

#	include "../../cache/proxy/Proxy.hpp"
#	include "Widget.hpp"

namespace page { namespace res { class Image; }}

namespace page { namespace gui
{
	/**
	 * A widget that displays an image.
	 */
	class ImageWidget : public Widget
	{
		IMPLEMENT_CLONEABLE(ImageWidget, Widget)

		/*-------------+
		| constructors |
		+-------------*/

		public:
		explicit ImageWidget(
			cache::Proxy<res::Image> const& image,
			math::Vec2               const& size = 0);

		/*-----------------+
		| Widget overrides |
		+-----------------*/

		private:
		WidgetSize CalcSize(const res::Theme &) const override;
		void DoDraw(DrawContext &) const override;

		/*-------------+
		| data members |
		+-------------*/

		/**
		 * The widget's image content.
		 */
		cache::Proxy<res::Image> image;

		/**
		 * The widget's size.
		 */
		math::Vec2 size;
	};
}}

#endif
