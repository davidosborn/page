#ifndef    page_local_wnd_Console_hpp
#   define page_local_wnd_Console_hpp

#	include "../util/class/Cloneable.hpp"
#	include <string>

namespace page { namespace wnd
{
	class Console : public virtual util::Cloneable<Console>
	{
		public:
		virtual void Put(char) = 0;
		virtual void Put(const std::string &) = 0;
	};
}}

#endif
