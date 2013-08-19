#ifndef    page_local_res_adapt_text_ParseTree_hpp
#   define page_local_res_adapt_text_ParseTree_hpp

#	include <string>
#	include <vector>

#	include "../../../util/raii/copy_ptr.hpp"

namespace page
{
	namespace res
	{
		class Stream;

		struct ParseTree
		{
			ParseTree(Stream &);

			class Node;
			// HACK: standard library container cannot hold incomplete types
			typedef std::vector<util::copy_ptr<Node>> Nodes;
			struct Node
			{
				Node(const std::string &, const std::string &);

				std::string key, value;
				Nodes children;
			};
			Nodes nodes;
		};
	}
}

#endif
