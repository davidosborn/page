#include <utility> // forward

namespace page { namespace res
{
////////// ScannerRegistry /////////////////////////////////////////////////////

	template <typename... RecordArgs>
		void ScannerRegistry::Register(RecordArgs &&... recordArgs)
	{
		Register(Record(std::forward<RecordArgs>(recordArgs)...));
	}
}}
