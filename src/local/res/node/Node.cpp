#include "Node.hpp"

namespace page
{
	namespace res
	{
		// construct
		Node::Node(const std::shared_ptr<Pipe> &pipe, const std::string &path, const std::string &mime, const std::string &tag, bool inspect) :
			pipe(pipe), path(path), mime(mime), tag(tag), inspect(inspect) {}
	}
}
