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
#include "../err/Exception.hpp"
#include "../err/report.hpp" // ReportWarning
#include "../log/Indenter.hpp"
#include "../util/functional/member.hpp" // member_of
#include "Driver.hpp" // Driver::~Driver
#include "DriverRegistry.hpp"

namespace page { namespace inp
{
////////// DriverRegistryRecord ///////////////////////////////////////////////

	DriverRegistryRecord::DriverRegistryRecord(
		std::type_info           const& type,
		DriverFactoryFunction    const& factoryFunction,
		DriverCompatibleFunction const& compatibleFunction,
		std::string              const& name,
		int                             priority) :
			type(type),
			factoryFunction(factoryFunction),
			compatibleFunction(compatibleFunction),
			name(name),
			priority(priority)
	{
		assert(factoryFunction    != nullptr);
		assert(compatibleFunction != nullptr);
	}

////////// DriverRegistry /////////////////////////////////////////////////////

	void DriverRegistry::Register(const Record &record)
	{
		// function to compare record priority
		auto greaterPriority(
			std::bind(
				std::greater<decltype(Record::priority)>(),
				std::bind(
					util::make_member_of(&Record::priority),
					std::placeholders::_1),
				std::bind(
					util::make_member_of(&Record::priority),
					std::placeholders::_2)));

		// add record to list
		records.insert(
			std::lower_bound(
				records.begin(),
				records.end(),
				record,
				greaterPriority),
			record);
	}

	std::unique_ptr<Driver> DriverRegistry::Make(wnd::Window &window) const
	{
		boost::optional<log::Indenter> indenter;
		if (*CVAR(logVerbose))
		{
			std::cout << "creating driver" << std::endl;
			indenter = boost::in_place();
		}

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
					return record.factoryFunction(window);
				}
				catch (const std::exception &e)
				{
					err::ReportWarning(e);
				}
			}

		THROW((err::Exception<err::InpModuleTag, err::NotFoundTag>("driver not found")))
	}
}}
