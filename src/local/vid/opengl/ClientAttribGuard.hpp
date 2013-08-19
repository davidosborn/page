#ifndef    page_local_vid_opengl_ClientAttribGuard_hpp
#   define page_local_vid_opengl_ClientAttribGuard_hpp

#	include "../../util/StateSaver.hpp"

namespace page
{
	namespace vid
	{
		namespace opengl
		{
			struct ClientAttribGuard : util::StateSaver<ClientAttribGuard>
			{
				friend class util::StateSaver<ClientAttribGuard>;
				typedef util::StateSaver<ClientAttribGuard> Base;

				// construct
				explicit ClientAttribGuard(bool enabled = true);

				private:
				// modifiers
				void Save();
				void Load();

				private:
				unsigned depth;
			};
		}
	}
}

#endif
