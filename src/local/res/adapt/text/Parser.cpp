#include <algorithm> // transform
#include <iostream> // clog

#include "../../../err/Exception.hpp"
#include "../../../log/manip.hpp" // Warning
#include "../../../util/functional/locale.hpp" // tolower_function
#include "../../../util/string/operations.hpp" // Join
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
					THROW((err::Exception<err::ResModuleTag, err::TokenTag, err::FormatTag>() <<
						err::errinfo_token(key)))
				const Handler &handler(iter->second);
				if (node.value.empty())
				{
					if (handler.constraint == needValue)
						THROW((err::Exception<err::ResModuleTag, err::ParserTag, err::FormatTag>("missing value")))
				}
				else if (!handler.callback || handler.constraint == noValue)
					std::clog << log::Warning << "unexpected value" << std::endl;
				if (handler.callback) handler.callback(node.value);
				Parse(node.children, key);
			}
		}
	}
}
