#include <algorithm> // lower_bound
#include <cassert>
#include <functional> // bind, greater
#include <iostream> // cout

#include <boost/optional.hpp>

#include "../cfg/vars.hpp"
#include "../err/report.hpp" // ReportWarning
#include "../log/Indenter.hpp"
#include "../util/functional/member.hpp" // member_of
#include "../wnd/Window.hpp" // typeid
#include "Device.hpp" // Device::~Device
#include "DeviceRegistry.hpp"

namespace page { namespace inp
{
////////// DeviceRegistryRecord ///////////////////////////////////////////////

	DeviceRegistryRecord::DeviceRegistryRecord(
		std::type_info        const& type,
		DeviceFactoryFunction const& factoryFunction,
		std::string           const& name) :
			type(type),
			factoryFunction(factoryFunction),
			name(name)
	{
		assert(factoryFunction != nullptr);
	}

////////// DeviceRegistry /////////////////////////////////////////////////////

	void DeviceRegistry::Register(const std::type_info &windowType, const Record &record)
	{
		// get window-type record for window type
		auto &windowTypeRecord(windowTypes.insert({windowType, {}}).first->second);

		// add record to list
		windowTypeRecord.records.push_back(record);
	}

	std::vector<std::unique_ptr<Device>> DeviceRegistry::MakeAll(wnd::Window &window) const
	{
		boost::optional<log::Indenter> indenter;
		if (*CVAR(logVerbose))
		{
			std::cout << "creating devices" << std::endl;
			indenter = boost::in_place();
		}

		std::vector<std::unique_ptr<Device>> devices;

		auto iter(windowTypes.find(typeid (window)));
		if (iter != windowTypes.end())
		{
			const auto &windowTypeRecord(iter->second);

			for (const auto &record : windowTypeRecord.records)
			{
				boost::optional<log::Indenter> indenter;
				if (*CVAR(logVerbose))
				{
					std::cout << "trying " << record.name << std::endl;
					indenter = boost::in_place();
				}

				try
				{
					devices.push_back(record.factoryFunction(window));
				}
				catch (const std::exception &e)
				{
					err::ReportWarning(e);
				}
			}
		}

		return devices;
	}
}}
