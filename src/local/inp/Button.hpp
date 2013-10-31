#ifndef    page_local_inp_Button_hpp
#   define page_local_inp_Button_hpp

#	include <string>

namespace page { namespace inp
{
	enum Button
	{
		leftButton,
		middleButton,
		rightButton
	};

	/**
	 * Returns a printable representation of the button.
	 */
	std::string GetName(Button);
}}

#endif
