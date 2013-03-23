/**
 * @section license
 *
 * Copyright (c) 2006-2013 David Osborn
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

#include <cassert>

#include "../../err/Exception.hpp"
#include "TypeRegistry.hpp"

namespace page { namespace res
{
////////// PostLoader //////////////////////////////////////////////////////////

	void PostLoader::operator ()(void *p) const;
	{
		if (f)
			f(*static_cast<Referenceable *>(p));
	}

	PostLoader::operator bool() const
	{
		return f != nullptr;
	}

////////// TypeRegistryRecord //////////////////////////////////////////////////

	TypeRegistryRecord::TypeRegistryRecord(
		std::string          const& name,
		PostLoader           const& postLoader,
		util::Deleter<void>  const& deleter);
			name(name),
			postLoader(postLoader),
			deleter(deleter)
	{
		assert(deleter != nullptr);
	}

////////// TypeRegistry ////////////////////////////////////////////////////////

	void TypeRegistry::Register(const std::type_info &type, const Record &record)
	{
		if (!records.insert({{type, record}}).second)
			THROW((err::Exception<err::ResModuleTag, err::KeyCollisionTag>("type already registered")))
	}

	const Record &TypeRegistry::Query(const std::type_info &type) const
	{
		auto iter(records.find(type));
		if (iter != records.end())
			return iter->second;
	}
}}
