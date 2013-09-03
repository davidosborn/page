#include <algorithm> // lower_bound
#include <cassert>
#include <functional> // bind, greater
#include <iostream> // clog

#include <boost/range/adaptor/indirected.hpp>

#include "../../err/Exception.hpp"
#include "../../util/functional/member.hpp" // member_of
#include "../../util/iterator/indirect_iterator.hpp"
#include "../../util/string/operations.hpp" // ToLower
#include "EncoderRegistry.hpp"

namespace page { namespace res { namespace clip
{
////////// EncoderRegistryRecord ///////////////////////////////////////////////

	EncoderRegistryRecord::EncoderRegistryRecord(
		std::string              const& name,
		EncoderFactoryFunction   const& factoryFunction,
		std::vector<std::string> const& formats,
		std::vector<std::string> const& extensions,
		bool                            binary,
		int                             priority) :
			name(name),
			factoryFunction(factoryFunction),
			formats(formats),
			extensions(extensions),
			defaultExtension(!extensions.empty() ? extensions.front() : ""),
			binary(binary),
			priority(priority)
	{
		assert(factoryFunction != nullptr);

		// ensure formats are lowercase
		for (const auto &format : formats)
			assert(format == util::ToLower(formats));

		// ensure extensions are lowercase
		for (const auto &extension : extensions)
			assert(extension == util::ToLower(extension));
	}

////////// EncoderRegistry /////////////////////////////////////////////////////

	void EncoderRegistry::Register(const Record &record)
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
		auto iter(
			records.insert(
				std::lower_bound(
					records.begin(),
					records.end(),
					record,
					greaterPriority),
				record));

		// associate formats with record
		for (const auto &format : record.formats)
		{
			auto &records(formats.insert({format, {}}).first->second);
			records.insert(
				std::lower_bound(
					util::make_indirect_iterator(records.begin()),
					util::make_indirect_iterator(records.end()),
					record,
					greaterPriority).base(),
				iter);
		}

		// associate extensions with record
		for (const auto &extension : record.extensions)
		{
			auto &records(extensions.insert({extension, {}}).first->second);
			records.insert(
				std::lower_bound(
					util::make_indirect_iterator(records.begin()),
					util::make_indirect_iterator(records.end()),
					record,
					greaterPriority).base(),
				iter);
		}
	}

	const typename EncoderRegistry::Record &EncoderRegistry::GetEncoder(
		std::string    const& format,
		std::string    const& extension,
		bool                  bestMatch) const
	{
		if (bestMatch)
		{
			// select encoder with matching format and extension
			if (!format.empty() && !extension.empty())
			{
				auto iter(formats.find(format));
				if (iter != formats.end())
				{
					auto records(boost::adaptors::indirect(iter->second));
					for (const auto &record : records)
						if (record.extensions.find(extension) != record.extensions.end())
							return record;
				}
			}

			// select encoder with matching format
			if (!format.empty())
			{
				auto iter(formats.find(format));
				if (iter != formats.end())
				{
					auto records(boost::adaptors::indirect(iter->second));
					for (const auto &record : records)
						return record;
				}

				std::clog << log::Warning << "unknown clip-encoder format: " << format << std::endl;
			}

			// select encoder with matching extension
			if (!extension.empty())
			{
				auto iter(extensions.find(extension));
				if (iter != extensions.end())
				{
					auto records(boost::adaptors::indirect(iter->second));
					for (const auto &record : records)
						return record;
				}

				std::clog << log::Warning << "unknown clip-encoder extension: " << extension << std::endl;
			}

			// select non-matching encoder
			for (const auto &record : records)
				return record;
		}
		else // exact match
		{
			if (!format.empty() && !extension.empty())
			{
				auto iter(formats.find(format));
				if (iter != formats.end())
				{
					auto records(boost::adaptors::indirect(iter->second));
					for (const auto &record : records)
						if (record.extensions.find(extension) != record.extensions.end())
							return record;
				}
			}
			else if (!format.empty())
			{
				auto iter(formats.find(format));
				if (iter != formats.end())
				{
					auto records(boost::adaptors::indirect(iter->second));
					for (const auto &record : records)
						return record;
				}
			}
			else if (!extension.empty())
			{
				auto iter(extensions.find(extension));
				if (iter != extensions.end())
				{
					auto records(boost::adaptors::indirect(iter->second));
					for (const auto &record : records)
						return record;
				}
			}
		}

		THROW((
			err::Exception<err::ResModuleTag, err::NotAvailableTag>(
				"no clip encoders available") <<
			boost::errinfo_file_name(path)))
	}
}}}
