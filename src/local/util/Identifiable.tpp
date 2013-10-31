#include "../err/Exception.hpp"

namespace page { namespace util
{
	/*-------------------------+
	| type-safe identification |
	+-------------------------*/

	template <typename T>
		T *GetPointerById(Identifiable::Id id)
	{
		return dynamic_cast<T *>(Identifiable::GetInstance(id));
	}

	template <typename T>
		T &GetReferenceById(Identifiable::Id id)
	{
		Identifiable *identifiable = Identifiable::GetInstance(id);
		if (!identifiable)
			THROW((err::Exception<err::UtilModuleTag, err::KeyTag>("invalid ID")))
		return dynamic_cast<T &>(*identifiable);
	}
}}
