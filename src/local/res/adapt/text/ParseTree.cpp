/**
 * @copyright
 *
 * Copyright (c) 2006-2014 David Osborn
 *
 * Permission is granted to use and redistribute this software in source and
 * binary form, with or without modification, subject to the following
 * conditions:
 *
 * 1. Redistributions in source form must retain the above copyright notice,
 *    this list of conditions, and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions, and the following disclaimer in the same place
 *    and form as other copyright, license, and disclaimer information.
 *
 * 3. Redistributions in binary form must also include an acknowledgement in the
 *    same place and form as other acknowledgements (such as the credits),
 *    similar in substance to the following:
 *
 *       Portions of this software are based on the work of David Osborn.
 *
 * This software is provided "as is", without any express or implied warranty.
 * In no event will the authors be liable for any damages arising out of the use
 * of this software.
 */

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
