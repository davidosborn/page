#ifndef    page_local_res_node_Node_hpp
#   define page_local_res_node_Node_hpp

#	include <memory> // shared_ptr
#	include <string>

namespace page
{
	namespace res
	{
		class Pipe;

		struct Node
		{
			// construct
			explicit Node(const std::shared_ptr<Pipe> & = std::shared_ptr<Pipe>(),
				const std::string &path = "", const std::string &mime = "",
				const std::string &tag = "", bool inspect = true);

			std::shared_ptr<Pipe> pipe;
			std::string path;
			std::string mime;
			std::string tag;
			bool inspect;
		};
	}
}

#endif
