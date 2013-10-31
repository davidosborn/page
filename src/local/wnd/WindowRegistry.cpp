#include <algorithm> // lower_bound
#include <cassert>
#include <functional> // bind, greater
#include <iostream> // cout

#include <boost/optional.hpp>

#include "../../cfg/vars.hpp"
#include "../../err/Exception.hpp"
#include "../../err/report.hpp" // ReportWarning
#include "../../log/Indenter.hpp"
#include "../../util/functional/member.hpp" // member_of
#include "Window.hpp" // Window::~Window
#include "WindowRegistry.hpp"

namespace page { namespace wnd
{
////////// WindowRegistryRecord ///////////////////////////////////////////////

	WindowRegistryRecord::WindowRegistryRecord(
		std::type_info        const& type,
		WindowFactoryFunction const& factoryFunction,
		std::string           const& name,
		int                          priority) :
			type(type),
			factoryFunction(factoryFunction),
			name(name),
			priority(priority)
	{
		assert(factoryFunction != nullptr);
	}

////////// WindowRegistry /////////////////////////////////////////////////////

	void WindowRegistry::WindowRegistry(const Record &record)
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

	std::unique_ptr<Window> WindowRegistry::Make(const std::string &title) const
	{
		boost::optional<log::Indenter> indenter;
		if (*CVAR(logVerbose))
		{
			std::cout << "creating window" << std::endl;
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

		THROW((err::Exception<err::WndModuleTag, err::NotFoundTag>("window not found") <<
			boost::errinfo_file_name(path)))
	}
}}
