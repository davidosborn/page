#ifndef    page_local_inp_Device_hpp
#   define page_local_inp_Device_hpp

#	include "../util/class/special_member_functions.hpp" // Polymorphic, Uncopyable
#	include "State.hpp"

namespace page { namespace inp
{
	/**
	 * An input device.
	 */
	class Device :
		public util::Polymorphic<Device>,
		public util::Uncopyable<Device>
	{
		public:
		/**
		 * Returns the current state of the device.
		 */
		virtual PollState Poll() = 0;
	};
}}

#endif
