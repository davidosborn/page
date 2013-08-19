#ifndef    page_local_vid_opengl_AttribGuard_hpp
#   define page_local_vid_opengl_AttribGuard_hpp

#	include "../../util/StateSaver.hpp"

namespace page
{
	namespace vid
	{
		namespace opengl
		{
			struct AttribGuard : util::StateSaver<AttribGuard>
			{
				friend class util::StateSaver<AttribGuard>;
				typedef util::StateSaver<AttribGuard> Base;

				// construct
				explicit AttribGuard(bool enabled = true);

				private:
				// modifiers
				void Save();
				void Load();

				unsigned depth;
			};
		}
	}
}

#endif
