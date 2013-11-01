#ifndef    page_local_wnd_win32_Console_hpp
#   define page_local_wnd_win32_Console_hpp

#	include <string>

#	include "../Console.hpp"

namespace page { namespace wnd { namespace win32
{
	class Console : public wnd::Console
	{
		IMPLEMENT_CLONEABLE(Console, wnd::Console)

		public:
		explicit Console(const std::string &title);

		public:
		void Put(char);
		void Put(const std::string &);
	};
}}}

#endif
