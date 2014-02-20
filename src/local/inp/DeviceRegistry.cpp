/**
 * @copyright
 *
 * Copyright (c) 2006-2014 David Osborn
 *
 * Permission is granted to use and redistribute this software in source and
 * binary form, with or without modification, subject to the following
 * conditions:
 *
 * 1. Redistributions in source form must retain the above copyright notice,
 *    this list of conditions, and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions, and the following disclaimer in the same place
 *    and form as other copyright, license, and disclaimer information.
 *
 * 3. Redistributions in binary form must also include an acknowledgement in the
 *    same place and form as other acknowledgements (such as the credits),
 *    similar in substance to the following:
 *
 *       Portions of this software are based on the work of David Osborn.
 *
 * This software is provided "as is", without any express or implied warranty.
 * In no event will the authors be liable for any damages arising out of the use
 * of this software.
 */

#include <algorithm> // lower_bound
#include <cassert>
#include <functional> // bind, greater
#include <iostream> // cout

#include <boost/optional.hpp>

#include "../cfg/vars.hpp"
#include "../err/report.hpp" // ReportWarning
#include "../log/Indenter.hpp"
#include "../util/functional/member.hpp" // member_of
#include "Device.hpp" // Device::~Device
#include "DeviceRegistry.hpp"

namespace page { namespace inp
{
////////// DeviceRegistryRecord ///////////////////////////////////////////////

	DeviceRegistryRecord::DeviceRegistryRecord(
		std::type_info           const& type,
		DeviceFactoryFunction    const& factoryFunction,
		DeviceCompatibleFunction const& compatibleFunction,
		std::string              const& name) :
			type(type),
			factoryFunction(factoryFunction),
			compatibleFunction(compatibleFunction),
			name(name)
	{
		assert(factoryFunction    != nullptr);
		assert(compatibleFunction != nullptr);
	}

////////// DeviceRegistry /////////////////////////////////////////////////////

	void DeviceRegistry::Register(const Record &record)
	{
		// add record to list
		records.push_back(record);
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

		for (const auto &record : records)
			if (record.compatibleFunction(window))
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

		return devices;
	}
}}
