#ifndef    page_local_gui_dialog_DebugWindow_hpp
#   define page_local_gui_dialog_DebugWindow_hpp

#	include <memory> // shared_ptr

#	include "../fwd.hpp" // TextWidget
#	include "../widget/container/Window.hpp"

namespace page { namespace gui
{
	/**
	 * A window that displays debug information such as frame rate and cache
	 * coherence.
	 */
	class DebugWindow : public Window
	{
		IMPLEMENT_CLONEABLE(DebugWindow, Widget)

		/*-------------+
		| constructors |
		+-------------*/

		public:
		DebugWindow();

		/*-------------+
		| data members |
		+-------------*/

		private:
		std::shared_ptr<gui::TextWidget>
			runTimeWidget,
			frameCountWidget,
			frameRateWidget,
			// FIXME: waiting on support for video statistics
			/*triangleCountWidget,
			triangleRateWidget,
			pixelCountWidget,
			pixelRateWidget,*/
			cacheTriesWidget,
			cacheMissesWidget,
			cacheCoherenceWidget;
	};
}}

#endif
