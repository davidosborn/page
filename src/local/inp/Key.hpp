#ifndef    page_local_inp_Key_hpp
#   define page_local_inp_Key_hpp

#	include <string>

namespace page { namespace inp
{
	enum Key
	{
		backspaceKey,
		deleteKey,
		downKey,
		enterKey,
		escapeKey,
		leftKey,
		pauseKey,
		printKey,
		recordKey,
		rightKey,
		tabKey,
		upKey
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
