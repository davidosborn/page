#ifndef    page_local_inp_Button_hpp
#   define page_local_inp_Button_hpp

#	include <string>

namespace page { namespace inp
{
	enum class Button
	{
		left,
		middle,
		right
	};

	/**
	 * Returns a printable representation of the button.
	 */
	std::string GetName(Button);
}}

#endif
