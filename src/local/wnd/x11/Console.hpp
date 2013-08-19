#ifndef    page_local_wnd_x11_Console_hpp
#   define page_local_wnd_x11_Console_hpp

#	include "../Console.hpp"

namespace page { namespace wnd { namespace x11
{
	class Console :
		public wnd::Console,
		public virtual util::Cloneable<Console, wnd::Console>
	{
		public:
		explicit Console(const std::string &title);

		public:
		void Put(char);
		void Put(const std::string &);
	};
}}}

#endif
