#include <functional> // bind
#include <utility> // forward

#include "../util/functional/cast.hpp" // dynamic_cast_function
#include "../util/functional/factory.hpp" // factory_function
#include "../util/functional/pointer.hpp" // address_of

namespace page { namespace inp
{
////////// DriverRegistry /////////////////////////////////////////////////////

	template <typename T, typename Window, typename... RecordArgs>
		void DriverRegistry::Register(RecordArgs &&... recordArgs)
	{
		Register(
			Record(
				typeid(T),
				std::bind(
					util::factory_function<T, Window &>(),
					std::bind(
						util::dynamic_cast_function<Window &, wnd::Window &>(),
						std::placeholders::_1)),
				std::bind(
					util::dynamic_cast_function<Window *, wnd::Window *>(),
					std::bind(
						util::address_of<wnd::Window>(),
						std::placeholders::_1)),
				std::forward<RecordArgs>(recordArgs)...));
	}
}}
