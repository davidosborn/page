#include <utility> // forward

#include "../util/functional/factory.hpp" // factory_function

namespace page { namespace inp
{
////////// DeviceRegistry /////////////////////////////////////////////////////

	template <typename T, typename Window, typename... RecordArgs>
		void DeviceRegistry::Register(RecordArgs &&... recordArgs)
	{
		Register(
			typeid (Window),
			Record(
				typeid (T),
				util::factory_function<T>(),
				std::forward<RecordArgs>(recordArgs)...));
	}
}}
