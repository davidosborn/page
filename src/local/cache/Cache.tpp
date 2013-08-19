#include "../util/class/typeinfo.hpp" // GetIncompleteTypeInfo

namespace page { namespace cache
{
	/*----------+
	| observers |
	+----------*/

	template <typename T>
		std::shared_ptr<const T>
			Cache::Fetch(const Signature &signature) const
	{
		return std::static_pointer_cast<const T>(
			Fetch(signature, util::GetIncompleteTypeInfo<T>()));
	}

	/*----------+
	| modifiers |
	+----------*/

	template <typename T>
		void Cache::Store(
			const Signature &signature,
			const std::shared_ptr<const T> &data,
			const std::function<void ()> &repair)
	{
		Store(signature, data, util::GetIncompleteTypeInfo<T>(), repair);
	}
}}
