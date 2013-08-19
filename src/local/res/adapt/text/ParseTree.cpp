#include <stack>

#include "../../../err/Exception.hpp"
#include "../../../util/string/operations.hpp" // Partition
#include "LineIterator.hpp"
#include "ParseTree.hpp"

namespace page
{
	namespace res
	{
		ParseTree::ParseTree(Stream &stream)
		{
			std::stack<Node *> stack;
			for (LineIterator line(stream); line; ++line)
			{
				if (line.Indentation() > stack.size())
					THROW((err::Exception<err::ResModuleTag, err::FormatTag>("unexpected indentation")))
				while (stack.size() > line.Indentation()) stack.pop();
				Nodes &nodes(!stack.empty() ? stack.top()->children : this->nodes);
				util::Partitioned part(util::Partition(*line));
				nodes.push_back(util::copy_ptr<Node>(new Node(part.first, part.second)));
				stack.push(nodes.back().get());
			}
		}

		ParseTree::Node::Node(const std::string &key, const std::string &value) :
			key(key), value(value) {}
	}
}
