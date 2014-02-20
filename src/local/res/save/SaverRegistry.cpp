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
#include <fstream> // ofstream
#include <functional> // bind, greater
#include <iostream> // clog

#include <boost/range/adaptor/indirected.hpp>

#include "../../err/Exception.hpp"
#include "../../util/functional/member.hpp" // member_of
#include "../../util/iterator/indirect_iterator.hpp"
#include "../../util/path/extension.hpp" // {Add,Get}Extension
#include "../../util/string/operations.hpp" // ToLower
#include "../type/TypeRegistry.hpp"
#include "SaverRegistry.hpp"

namespace page { namespace res
{
////////// Saver ///////////////////////////////////////////////////////////////

	void Saver::operator ()(const void *p, std::ostream &os) const;
	{
		if (f)
			f(*static_cast<Referenceable *>(p), os);
	}

	Saver::operator bool() const
	{
		return f != nullptr;
	}

////////// SaverRegistryRecord /////////////////////////////////////////////////

	SaverRegistryRecord::SaverRegistryRecord(
		std::string              const& name,
		Saver                    const& saver,
		std::vector<std::string> const& formats,
		std::vector<std::string> const& extensions,
		bool                            binary,
		int                             priority) :
			name(name),
			saver(saver),
			formats(formats),
			extensions(extensions),
			defaultExtension(!extensions.empty() ? extensions.front() : ""),
			binary(binary),
			priority(priority)
	{
		assert(saver != nullptr);

		// ensure formats are lowercase
		for (const auto &format : formats)
			assert(format == util::ToLower(formats));

		// ensure extensions are lowercase
		for (const auto &extension : extensions)
			assert(extension == util::ToLower(extension));
	}

////////// SaverRegistry ///////////////////////////////////////////////////////

	void SaverRegistry::Register(const std::type_info &type, const Record &record)
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

		// get type record for type
		auto &typeRecord(types.insert({type, {}}).first->second);

		// add record to list
		auto iter(
			typeRecord.records.insert(
				std::lower_bound(
					typeRecord.records.begin(),
					typeRecord.records.end(),
					record,
					greaterPriority),
				record));

		// associate formats with record
		for (const auto &format : record.formats)
		{
			auto &records(typeRecord.formats.insert({format, {}}).first->second);
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
			auto &records(typeRecord.extensions.insert({extension, {}}).first->second);
			records.insert(
				std::lower_bound(
					util::make_indirect_iterator(records.begin()),
					util::make_indirect_iterator(records.end()),
					record,
					greaterPriority).base(),
				iter);
		}
	}

	std::string SaverRegistry::Save(
		std::type_info const& type,
		T              const* resource,
		std::string           path,
		std::string    const& format,
		bool                  bestMatch) const
	{
		// select saver
		auto extension(util::ToLower(util::GetExtension(path)));
		const auto &record(GetSaver(type, format, extension, bestMatch));

		// adjust path extension to match saver
		if (!record.extensions.empty() &&
			record.extensions.find(extension) == record.extensions.end())
				path = util::AddExtension(path, record.defaultExtension);

		// open file for writing
		std::ofstream fs(path, record.binary ? fs.binary : fs.text);
		if (!fs)
			THROW((
				err::Exception<err::ResModuleTag, err::FileAccessTag>(
					"failed to open file for writing") <<
				boost::errinfo_file_name(normPath)))

		// execute saver
		record.saver(resource, fs);
		std::cout << GLOBAL(TypeRegistry).Query(type).name << " saved as " << path << std::endl;
		return path;
	}

	const typename SaverRegistry::Record &SaverRegistry::GetSaver(
		std::type_info const& type,
		std::string    const& format,
		std::string    const& extension,
		bool                  bestMatch) const
	{
		auto iter(types.find(type));
		if (iter != types.end())
		{
			const auto &typeRecord(iter->second);

			if (bestMatch)
			{
				// select saver with matching format and extension
				if (!format.empty() && !extension.empty())
				{
					auto iter(typeRecord.formats.find(format));
					if (iter != typeRecord.formats.end())
					{
						auto records(boost::adaptors::indirect(iter->second));
						for (const auto &record : records)
							if (record.extensions.find(extension) != record.extensions.end())
								return record;
					}
				}

				// select saver with matching format
				if (!format.empty())
				{
					auto iter(typeRecord.formats.find(format));
					if (iter != typeRecord.formats.end())
					{
						auto records(boost::adaptors::indirect(iter->second));
						for (const auto &record : records)
							return record;
					}

					std::clog << log::Warning << "unknown " << GLOBAL(TypeRegistry).Query(type).name << " format: " << format << std::endl;
				}

				// select saver with matching extension
				if (!extension.empty())
				{
					auto iter(typeRecord.extensions.find(extension));
					if (iter != typeRecord.extensions.end())
					{
						auto records(boost::adaptors::indirect(iter->second));
						for (const auto &record : records)
							return record;
					}

					std::clog << log::Warning << "unknown " << GLOBAL(TypeRegistry).Query(type).name << " extension: " << extension << std::endl;
				}

				// select non-matching saver
				for (const auto &record : typeRecord.records)
					return record;
			}
			else // exact match
			{
				if (!format.empty() && !extension.empty())
				{
					auto iter(typeRecord.formats.find(format));
					if (iter != typeRecord.formats.end())
					{
						auto records(boost::adaptors::indirect(iter->second));
						for (const auto &record : records)
							if (record.extensions.find(extension) != record.extensions.end())
								return record;
					}
				}
				else if (!format.empty())
				{
					auto iter(typeRecord.formats.find(format));
					if (iter != typeRecord.formats.end())
					{
						auto records(boost::adaptors::indirect(iter->second));
						for (const auto &record : records)
							return record;
					}
				}
				else if (!extension.empty())
				{
					auto iter(typeRecord.extensions.find(extension));
					if (iter != typeRecord.extensions.end())
					{
						auto records(boost::adaptors::indirect(iter->second));
						for (const auto &record : records)
							return record;
					}
				}
			}
		}

		THROW((
			err::Exception<err::ResModuleTag, err::NotAvailableTag>(
				"no " + GLOBAL(TypeRegistry).Query(type).name + " savers available") <<
			boost::errinfo_file_name(path)))
	}
}}
