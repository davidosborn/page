#include <cassert>
#include <iostream> // cout
#include <vector>

#include "../../log/Indenter.hpp"
#include "../Device.hpp" // Device::~Device
#include "registry.hpp" // DeviceFactory, Devices

namespace page
{
	namespace inp
	{
		namespace
		{
			struct Registry
			{
				struct Device
				{
					DeviceFactory factory;
					std::string name;
				};
				typedef std::vector<Device> Devices;
				Devices devices;
			};
			inline Registry &GetRegistry()
			{
				static Registry reg;
				return reg;
			}
		}

		// creation
		Devices MakeDevices(wnd::Window &wnd)
		{
			const Registry &reg(GetRegistry());
			Devices devices;
			for (Registry::Devices::const_iterator iter(reg.devices.begin()); iter != reg.devices.end(); ++iter)
			{
				std::cout << "loading device: " << iter->name << std::endl;
				log::Indenter indenter;
				Device *device = iter->factory(wnd);
				assert(device);
				devices.push_back(std::shared_ptr<Device>(device));
			}
			return devices;
		}

		// registration
		void RegisterDevice(const DeviceFactory &factory, const std::string &name)
		{
			assert(factory);
			Registry::Device device = {factory, name};
			GetRegistry().devices.push_back(device);
		}
	}
}
