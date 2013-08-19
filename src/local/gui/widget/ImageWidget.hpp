#ifndef    page_local_ui_widget_ImageWidget_hpp
#   define page_local_ui_widget_ImageWidget_hpp

#	include "../../cache/fwd.hpp" // Proxy
#	include "../../util/raii/copy_ptr.hpp"
#	include "Widget.hpp"

namespace page { namespace res { class Image; }}

namespace page { namespace ui
{
	/**
	 * A widget that displays an image.
	 */
	class ImageWidget :
		public Widget,
		public virtual util::Cloneable<ImageWidget, Widget>
	{
		/*--------------------------+
		| constructors & destructor |
		+--------------------------*/

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
		util::copy_ptr<cache::Proxy<res::Image>> image;

		/**
		 * The widget's size.
		 */
		math::Vec2 size;
	};
}}

#endif
