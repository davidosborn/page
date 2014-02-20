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
