#include "../err/Exception.hpp"

namespace page
{
	namespace util
	{
		// identification
		template <typename T> T *PointerFromId(Identifiable::Id id)
		{
			return dynamic_cast<T *>(Identifiable::FromId(id));
		}
		template <typename T> T &ReferenceFromId(Identifiable::Id id)
		{
			Identifiable *identifiable = Identifiable::FromId(id);
			if (!identifiable)
				THROW((err::Exception<err::UtilModuleTag, err::KeyTag>("invalid ID")))
			return dynamic_cast<T &>(*identifiable);
		}
	}
}
