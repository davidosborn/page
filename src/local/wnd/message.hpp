#ifndef    page_local_wnd_message_hpp
#   define page_local_wnd_message_hpp

#	include <string>

namespace page { namespace wnd
{
	/**
	 * The type of message for the message box, which affects its default
	 * title and the icon.
	 */
	enum class MessageType
	{
		info,
		error,
		warning
	};

	/**
	 * Displays a message box to the user, with a default title.
	 */
	void Message(const std::string &, MessageType = MessageType::info);

	/**
	 * Displays a message box to the user.
	 */
	void Message(const std::string &, MessageType, const std::string &title);
}}

#endif
