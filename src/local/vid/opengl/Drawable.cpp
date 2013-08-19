#include "../../cfg/vars.hpp"
#include "ext.hpp" // ARB_vertex_buffer_object
#include "VertexArray.hpp"
#include "VertexBuffer.hpp"

namespace page
{
	namespace vid
	{
		namespace opengl
		{
			Drawable *MakeDrawable(const res::Mesh &mesh, bool dynamic)
			{
				return
					*CVAR(opengl)::renderVbo && haveArbVertexBufferObject ?
					static_cast<Drawable *>(new VertexBuffer(mesh,
						dynamic ? GL_DYNAMIC_DRAW_ARB : GL_STATIC_DRAW_ARB)) :
					static_cast<Drawable *>(new VertexArray(mesh));
			}
		}
	}
}
