#include <utility> // forward

#include "../util/functional/factory.hpp" // factory_function

namespace page { namespace wnd
{
////////// WindowRegistry /////////////////////////////////////////////////////

	template <typename T, typename... RecordArgs>
		void WindowRegistry::Register(RecordArgs &&... recordArgs)
	{
		Register(
			Record(
				typeid(T),
				util::factory_function<T>(),
				std::forward<RecordArgs>(recordArgs)...));
	}
}}
