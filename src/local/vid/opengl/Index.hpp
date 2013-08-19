#ifndef    page_local_vid_opengl_Index_hpp
#   define page_local_vid_opengl_Index_hpp

#	include <GL/gl.h> // GLshort

namespace page
{
	namespace res { class Mesh; }

	namespace vid
	{
		namespace opengl
		{
			typedef GLshort Index;

			template <typename OutputIterator> void InitIndices(OutputIterator, const res::Mesh &);
			void DrawIndices(const Index *, unsigned n);
		}
	}
}

#	include "Index.tpp"
#endif
