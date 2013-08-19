#ifndef    page_local_log_manip_hpp
#   define page_local_log_manip_hpp

#	include <iosfwd> // ostream

namespace page
{
	namespace log
	{
		/*------+
		| lines |
		+------*/

		std::ostream &Clear(std::ostream &);

		/*------------+
		| indentation |
		+------------*/

		std::ostream &Indent(std::ostream &);
		std::ostream &Dedent(std::ostream &);

		/*---------------+
		| classification |
		+---------------*/

		std::ostream &Error(std::ostream &);
		std::ostream &Warning(std::ostream &);
	}
}

#endif
