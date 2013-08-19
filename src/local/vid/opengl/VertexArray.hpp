#ifndef    page_local_vid_opengl_VertexArray_hpp
#   define page_local_vid_opengl_VertexArray_hpp

#	include "Drawable.hpp"
#	include "Index.hpp"

namespace page
{
	namespace vid
	{
		namespace opengl
		{
			struct VertexArray : Drawable
			{
				explicit VertexArray(const res::Mesh &);

				void Update(const phys::Skin &);
				void Draw(const VertexFormat &) const;

				private:
				typedef std::vector<Index> Indices;
				Indices indices;
				typedef std::vector<Vertex> Vertices;
				Vertices vertices;
			};
		}
	}
}

#endif
