#include <utility> // forward

namespace page { namespace res
{
////////// EncoderRegistry /////////////////////////////////////////////////////

	template <typename... RecordArgs>
		void EncoderRegistry::Register(RecordArgs &&... recordArgs)
	{
		Register(Record(std::forward<RecordArgs>(recordArgs)...));
	}
}}
