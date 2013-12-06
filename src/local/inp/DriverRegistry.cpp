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
