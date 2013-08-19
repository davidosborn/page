#ifndef    page_local_vid_opengl_Vertex_hpp
#   define page_local_vid_opengl_Vertex_hpp

#	include <vector>

#	include <GL/gl.h> // GLfloat

namespace page
{
	namespace phys { class Skin; }
	namespace res { class Mesh; }

	namespace vid
	{
		namespace opengl
		{
#	pragma pack(push, 1)
			struct Vertex
			{
				GLfloat
					uv[2][3], // texture coordinates
					no[3],    // normal
					tan[3],   // tangent
					co[3],    // position
					padding;
			};
#	pragma pack(pop)

			template <typename OutputIterator> void InitVertices(OutputIterator, const res::Mesh &);
			template <typename Iterator> void UpdateVertices(Iterator, const phys::Skin &);

			struct VertexFormat
			{
				// construct
				VertexFormat();

				typedef std::vector<unsigned> UvIndices;
				UvIndices uvIndices;
				bool normal, tangent;
			};
			void PrepVertices(const Vertex *, const VertexFormat & = VertexFormat());
		}
	}
}

#	include "Vertex.tpp"
#endif
