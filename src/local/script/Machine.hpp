// cooperative virtual machine base class

#ifndef    page_local_script_Machine_hpp
#   define page_local_script_Machine_hpp

#	include "../util/class/special_member_functions.hpp" // Uncopyable

namespace page
{
	namespace res { class Script; }

	namespace script
	{
		class Process;

		struct Machine : util::Uncopyable<Machine>
		{
			// destroy
			virtual ~Machine();

			// execution
			virtual Process *Open(const res::Script &) = 0;
		};
	}
}

#endif
