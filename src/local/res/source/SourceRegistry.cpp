/**
 * @section license
 *
 * Copyright (c) 2006-2012 David Osborn
 *
 * Permission is granted to use and redistribute this software in source and
 * binary form, with or without modification, subject to the following
 * conditions:
 *
 * 1. Redistributions in source form must retain the above copyright notice,
 *    this list of conditions, and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions, and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution, and in the same
 *    place and form as other copyright, license, and disclaimer information.
 *
 * As a special exception, distributions of derivative works in binary form may
 * include an acknowledgement in place of the above copyright notice, this list
 * of conditions, and the following disclaimer in the documentation and/or other
 * materials provided with the distribution, and in the same place and form as
 * other acknowledgements, similar in substance to the following:
 *
 *    Portions of this software are based on the work of David Osborn.
 *
 * This software is provided "as is", without any express or implied warranty.
 * In no event will the authors be liable for any damages arising out of the use
 * of this software.
 */

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
