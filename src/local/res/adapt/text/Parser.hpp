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
