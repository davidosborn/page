#include <utility> // forward

#include "../../util/functional/factory.hpp" // factory_function

namespace page { namespace res
{
////////// SourceRegistry //////////////////////////////////////////////////////

	template <typename T, typename... RecordArgs>
		void SourceRegistry::Register(RecordArgs &&... recordArgs)
	{
		Register(
			Record(
				util::factory_function<T>(),
				T::CheckPath,
				std::forward<RecordArgs>(recordArgs)...));
	}
}}
