#include <algorithm> // lower_bound
#include <cassert>
#include <functional> // bind, greater

#include "../../err/Exception.hpp"
#include "../../util/functional/member.hpp" // member_of
#include "SourceRegistry.hpp"

namespace page { namespace res
{
////////// SourceRegistryRecord ////////////////////////////////////////////////

	SourceRegistryRecord::SourceRegistryRecord(
		SourceFactoryFunction    const& factoryFunction,
		SourceCompatibleFunction const& compatibleFunction,
		int                             priority) :
			factoryFunction(factoryFunction),
			compatibleFunction(compatibleFunction),
			priority(priority)
	{
		assert(factoryFunction    != nullptr);
		assert(compatibleFunction != nullptr);
	}

////////// SourceRegistry //////////////////////////////////////////////////////

	void SourceRegistry::SourceRegistry(const Record &record)
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

	std::unique_ptr<Source> SourceRegistry::Make(const std::string &path) const
	{
		for (const auto &record : records)
			if (record.compatibleFunction(path))
				return record.factoryFunction(path);

		THROW((err::Exception<err::ResModuleTag, err::NotFoundTag>("source not found") <<
			boost::errinfo_file_name(path)))
	}
}}
