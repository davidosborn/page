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

#ifndef    page_local_res_adapt_text_Parser_hpp
#   define page_local_res_adapt_text_Parser_hpp

#	include <functional> // bind, function, ref
#	include <string>
#	include <unordered_map>

#	include "ParseTree.hpp" // ParseTree::Nodes

namespace page
{
	namespace res
	{
		class ParseTree;
		class Stream;

		struct Parser
		{
			enum Constraint
			{
				noValue,    // providing a value is an error
				allowValue, // values are permitted but not required
				needValue   // values are required
			};
			typedef std::function<void (const std::string &value)> Callback;
			void Register(const std::string &key, const Callback & = 0, Constraint = allowValue);

			void Parse(Stream &) const;

			private:
			void Parse(const ParseTree::Nodes &, const std::string &parent = std::string()) const;

			struct Handler
			{
				Callback callback;
				Constraint constraint;
			};
			typedef std::unordered_map<std::string, Handler> Handlers;
			Handlers handlers;
		};

		// function adapter
		template <typename Function, typename T> inline Parser::Callback Bind(const Function &function, T &target)
		{
			return std::bind(function, std::ref(target), std::placeholders::_1);
		}
	}
}

#endif
