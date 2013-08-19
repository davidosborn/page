#include <memory> // unique_ptr

#include "../../util/cpp.hpp" // STRINGIZE
#include "../node/Node.hpp"
#include "../pipe/Pipe.hpp" // Pipe::Open
#include "../pipe/Stream.hpp"
#include "Registry.hpp" // REGISTER_SCANNER

namespace page
{
	namespace res
	{
		bool ScanOgg(const std::shared_ptr<const Pipe> &pipe, const ScanCallback &cb)
		{
			const std::unique_ptr<Stream> stream(pipe->Open());
			// FIXME: implement
			return false;
		}

		// FIXME: waiting for implementation
		/*REGISTER_SCANNER(
			STRINGIZE(OGG_NAME),
			ScanOgg,
			{"application/ogg", "audio/ogg", "video/ogg"},
			{"ogg"})*/
	}
}
