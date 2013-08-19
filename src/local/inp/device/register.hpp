#ifndef    page_local_inp_device_register_hpp
#   define page_local_inp_device_register_hpp

#	include "registry.hpp" // RegisterDevice

namespace page
{
	namespace inp
	{
#	define REGISTER_DEVICE(factory, name) \
		namespace { DeviceRegisterer _regDevice(factory, name); }

		struct DeviceRegisterer
		{
			DeviceRegisterer(const DeviceFactory &factory, const std::string &name)
				{ RegisterDevice(factory, name); }
		};
	}
}

#endif
