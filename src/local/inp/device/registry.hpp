#ifndef    page_local_inp_device_registry_hpp
#   define page_local_inp_device_registry_hpp

#	include <functional> // function
#	include <memory> // shared_ptr
#	include <string>
#	include <vector>

namespace page
{
	namespace wnd { class Window; }

	namespace inp
	{
		class Device;

		// factory type
		typedef std::function<Device *(wnd::Window &)> DeviceFactory;

		// creation
		typedef std::vector<std::shared_ptr<Device>> Devices;
		Devices MakeDevices(wnd::Window &);

		// registration
		void RegisterDevice(const DeviceFactory &, const std::string &name);
	}
}

#endif
