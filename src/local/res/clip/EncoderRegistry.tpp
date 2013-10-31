#include <utility> // forward

#include "../../util/functional/factory.hpp" // factory_function

namespace page { namespace res
{
////////// EncoderRegistry /////////////////////////////////////////////////////

	template <typename T, typename... RecordArgs>
		void EncoderRegistry::Register(RecordArgs &&... recordArgs)
	{
		Register(
			Record(
				util::factory_function<T, const EncoderCallback &, const math::Vec2u &, float, float>(),
				std::forward<RecordArgs>(recordArgs)...));
	}
}}
