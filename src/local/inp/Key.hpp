#ifndef    page_local_inp_Key_hpp
#   define page_local_inp_Key_hpp

#	include <string>

namespace page { namespace inp
{
	enum class Key
	{
		backspace,
		delete_,
		down,
		enter,
		escape,
		left,
		pause,
		print,
		record,
		right,
		tab,
		up
	};

	/**
	 * Returns a printable representation of the key.
	 */
	std::string GetName(Key);

	/**
	 * Returns a printable representation of the character.
	 */
	std::string GetName(char);
}}

#endif
