#include <utility> // forward

#include "../../util/class/typeinfo.hpp" // GetIncompleteTypeInfo

namespace page { namespace res
{
////////// LoaderRegistry //////////////////////////////////////////////////////

	template <typename T, typename... RecordArgs>
		void LoaderRegistry::Register(RecordArgs &&... recordArgs)
	{
		Register(
			util::GetIncompleteTypeInfo<T>(),
			Record(std::forward<RecordArgs>(recordArgs)...));
	}

	template <typename T>
		const Loader &LoaderRegistry::GetLoader(const Node &node) const
	{
		return GetLoader(util::GetIncompleteTypeInfo<T>(), node);
	}
}}
