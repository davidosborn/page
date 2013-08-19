#include "Index.hpp" // Index

namespace page
{
	namespace vid
	{
		namespace opengl
		{
			void DrawIndices(const Index *indices, unsigned n)
			{
				glDrawElements(GL_TRIANGLES, n, GL_UNSIGNED_SHORT, indices);
			}
		}
	}
}
