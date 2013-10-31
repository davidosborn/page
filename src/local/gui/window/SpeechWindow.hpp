#ifndef    page_local_gui_dialog_SpeechWindow_hpp
#   define page_local_gui_dialog_SpeechWindow_hpp

#	include "../widget/container/Window.hpp"

namespace page { namespace gui
{
	/**
	 * A window used to display the portrait and words of a character while
	 * they are speaking.
	 */
	class SpeechWindow : public Window
	{
		IMPLEMENT_CLONEABLE(SpeechWindow, Widget)

		/*-------------+
		| constructors |
		+-------------*/

		public:
		SpeechWindow();
	};
}}

#endif
