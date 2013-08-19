// program object state saver
// NOP if ARB_shader_objects not supported

#ifndef    page_local_vid_opengl_ProgramSaver_hpp
#   define page_local_vid_opengl_ProgramSaver_hpp

#	include <GL/gl.h> // glext.h
#	include <GL/glext.h> // GLhandleARB

#	include "../../util/StateSaver.hpp"

namespace page
{
	namespace vid
	{
		namespace opengl
		{
			struct ProgramSaver : util::StateSaver<ProgramSaver>
			{
				friend class util::StateSaver<ProgramSaver>;
				typedef util::StateSaver<ProgramSaver> Base;

				// construct
				explicit ProgramSaver(bool enabled = true);

				private:
				// modifiers
				void Save();
				void Load();

				GLhandleARB program;
			};
		}
	}
}

#endif
