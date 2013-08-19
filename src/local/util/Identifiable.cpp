#include "Identifiable.hpp"

namespace page
{
	namespace util
	{
		// construct/copy/destroy/assign
		Identifiable::Identifiable() : id(MakeId())
		{
			GetIds().insert(std::make_pair(id, this));
		}
		Identifiable::Identifiable(const Identifiable &other) : id(MakeId())
		{
			GetIds().insert(std::make_pair(id, this));
		}
		Identifiable::~Identifiable()
		{
			GetIds().erase(id);
		}
		Identifiable &Identifiable::operator =(const Identifiable &) {}

		// identification
		Identifiable::Id Identifiable::GetId() const
		{
			return id;
		}
		Identifiable *Identifiable::FromId(Id id)
		{
			Ids::const_iterator iter(GetIds().find(id));
			return iter != GetIds().end() ? iter->second : 0;
		}

		// identity generation
		Identifiable::Id Identifiable::MakeId()
		{
			static Id id = 1;
			return id++;
		}

		// identity mapping
		Identifiable::Ids &Identifiable::GetIds()
		{
			static Ids ids;
			return ids;
		}
	}
}
