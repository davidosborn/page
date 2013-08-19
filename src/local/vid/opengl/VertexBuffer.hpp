#ifndef    page_local_vid_opengl_VertexBuffer_hpp
#   define page_local_vid_opengl_VertexBuffer_hpp

#	include <GL/gl.h> // GLenum, GLuint

#	include "Drawable.hpp"

namespace page
{
	namespace vid
	{
		namespace opengl
		{
			struct VertexBuffer : Drawable
			{
				VertexBuffer(const res::Mesh &, GLenum usage);
				~VertexBuffer();

				void Update(const phys::Skin &);
				void Draw(const VertexFormat &) const;

				private:
				unsigned numIndices;
				GLuint indices, vertices;
			};
		}
	}
}

#endif
