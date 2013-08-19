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
		THROW((err::Exception<err::ResModuleTag, err::KeyNotFoundTag>("type not registered")))
	}
}}
