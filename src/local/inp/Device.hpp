#ifndef    page_local_inp_Device_hpp
#   define page_local_inp_Device_hpp

#	include "../util/class/special_member_functions.hpp" // Uncopyable
#	include "State.hpp"

namespace page
{
	namespace inp
	{
		struct Device : util::Uncopyable<Device>
		{
			// destroy
			virtual ~Device();

			// state query
			virtual State Poll() = 0;
		};
	}
}

#endif
