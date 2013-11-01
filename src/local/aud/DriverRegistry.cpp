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
#include "../wnd/Window.hpp" // typeid
#include "Driver.hpp" // Driver::~Driver
#include "DriverRegistry.hpp"

namespace page { namespace aud
{
////////// DriverRegistryRecord ///////////////////////////////////////////////

	DriverRegistryRecord::DriverRegistryRecord(
		std::type_info        const& type,
		DriverFactoryFunction const& factoryFunction,
		std::string           const& name,
		int                          priority) :
			type(type),
			factoryFunction(factoryFunction),
			name(name),
			priority(priority)
	{
		assert(factoryFunction != nullptr);
	}

////////// DriverRegistry /////////////////////////////////////////////////////

	void DriverRegistry::Register(const std::type_info &windowType, const Record &record)
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

		// get window-type record for window type
		auto &windowTypeRecord(windowTypes.insert({windowType, {}}).first->second);

		// add record to list
		windowTypeRecord.records.insert(
			std::lower_bound(
				windowTypeRecord.records.begin(),
				windowTypeRecord.records.end(),
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

		// FIXME: what we need to do here is take all the records for the
		// derived window type and all the records for the base window type
		// (ie: Window) and merge the lists (which are each sorted, but we have
		// to sort them together).  Then we can iterate over the result.

		auto iter(windowTypes.find(typeid(window)));
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
					return record.factoryFunction(window);
				}
				catch (const std::exception &e)
				{
					err::ReportWarning(e);
				}
			}
		}

		THROW((err::Exception<err::InpModuleTag, err::NotFoundTag>("driver not found")))
	}
}}
