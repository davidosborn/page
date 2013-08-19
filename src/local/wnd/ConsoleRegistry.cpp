#include <algorithm> // lower_bound
#include <cassert>
#include <functional> // bind, greater

#include "../../err/Exception.hpp"
#include "../../util/functional/member.hpp" // member_of
#include "ConsoleRegistry.hpp"

namespace page { namespace wnd
{
////////// ConsoleRegistryRecord ///////////////////////////////////////////////

	ConsoleRegistryRecord::ConsoleRegistryRecord(
		std::type_info         const& type,
		ConsoleFactoryFunction const& factoryFunction,
		std::string            const& name,
		int                           priority) :
			type(type),
			factoryFunction(factoryFunction),
			name(name),
			priority(priority)
	{
		assert(factoryFunction != nullptr);
	}

////////// ConsoleRegistry /////////////////////////////////////////////////////

	void ConsoleRegistry::ConsoleRegistry(const Record &record)
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

	std::unique_ptr<Console> ConsoleRegistry::Make(const std::string &title) const
	{
		boost::optional<log::Indenter> indenter;
		if (*CVAR(logVerbose))
		{
			std::cout << "creating console" << std::endl;
			indenter = boost::in_place();
		}

		for (const auto &record : records)
		{
			boost::optional<log::Indenter> indenter;
			if (*CVAR(logVerbose))
			{
				std::cout << "trying " << record.name << std::endl;
				indenter = boost::in_place();
			}

			try
			{
				return record.factoryFunction(title);
			}
			catch (const std::exception &e)
			{
				err::ReportWarning(e);
			}
		}

		THROW((err::Exception<err::ResModuleTag, err::NotFoundTag>("console not found") <<
			boost::errinfo_file_name(path)))
	}
}}
