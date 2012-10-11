/**
 * @section license
 *
 * Copyright (c) 2006-2012 David Osborn
 *
 * Permission is granted to use and redistribute this software in source and
 * binary form, with or without modification, subject to the following
 * conditions:
 *
 * 1. Redistributions in source form must retain the above copyright notice,
 *    this list of conditions, and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions, and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution, and in the same
 *    place and form as other copyright, license, and disclaimer information.
 *
 * As a special exception, distributions of derivative works in binary form may
 * include an acknowledgement in place of the above copyright notice, this list
 * of conditions, and the following disclaimer in the documentation and/or other
 * materials provided with the distribution, and in the same place and form as
 * other acknowledgements, similar in substance to the following:
 *
 *    Portions of this software are based on the work of David Osborn.
 *
 * This software is provided "as is", without any express or implied warranty.
 * In no event will the authors be liable for any damages arising out of the use
 * of this software.
 */

#include <algorithm> // transform
#include <iostream> // clog
#include "../../../err/exception/throw.hpp" // THROW
#include "../../../log/manip.hpp" // Warning
#include "../../../util/functional.hpp" // tolower_function
#include "../../../util/string.hpp" // Join
#include "Parser.hpp"

namespace page
{
	namespace res
	{
		void Parser::Register(const std::string &key, const Callback &callback, Constraint constraint)
		{
			Handler handler = {callback, constraint};
			handlers.insert(std::make_pair(key, handler));
		}

		void Parser::Parse(Stream &stream) const
		{
			Parse(ParseTree(stream).nodes);
		}
		void Parser::Parse(const ParseTree::Nodes &nodes, const std::string &parent) const
		{
			for (ParseTree::Nodes::const_iterator iter(nodes.begin()); iter != nodes.end(); ++iter)
			{
				const ParseTree::Node &node(**iter);
				std::string key(node.key);
				std::transform(key.begin(), key.end(), key.begin(), util::tolower_function<char>());
				if (!parent.empty()) key.insert(0, parent + '.');
				Handlers::const_iterator iter(handlers.find(key));
				if (iter == handlers.end())
					THROW err::FormatException<err::ResourceTag>(util::Join("bad token: ", key));
				const Handler &handler(iter->second);
				if (node.value.empty())
				{
					if (handler.constraint == needValue)
						THROW err::FormatException<err::ResourceTag>("missing value");
				}
				else if (!handler.callback || handler.constraint == noValue)
					std::clog << log::Warning << "unexpected value" << std::endl;
				if (handler.callback) handler.callback(node.value);
				Parse(node.children, key);
			}
		}
	}
}
