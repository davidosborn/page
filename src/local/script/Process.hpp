#ifndef    page_local_script_Process_hpp
#   define page_local_script_Process_hpp

#	include "../util/class/special_member_functions.hpp" // Uncopyable

namespace page
{
	namespace script
	{
		struct Process : util::Uncopyable<Process>
		{
			// destroy
			virtual ~Process();

			// execution
			virtual bool Continue(float deltaTime) = 0;
		};
	}
}

#endif
