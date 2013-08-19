#ifndef    page_local_vid_opengl_Drawable_hpp
#   define page_local_vid_opengl_Drawable_hpp

#	include "Vertex.hpp" // VertexFormat

namespace page
{
	namespace phys { class Skin; }
	namespace res { class Mesh; }

	namespace vid
	{
		namespace opengl
		{
			struct Drawable
			{
				virtual void Update(const phys::Skin &) = 0;
				virtual void Draw(const VertexFormat & = VertexFormat()) const = 0;
			};

			// factory function
			// NOTE: dynamic is only meaningful for vertex buffers
			Drawable *MakeDrawable(const res::Mesh &, bool dynamic = false);
		}
	}
}

#endif
