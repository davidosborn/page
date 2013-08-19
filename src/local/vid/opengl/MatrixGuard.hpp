#ifndef    page_local_vid_opengl_MatrixGuard_hpp
#   define page_local_vid_opengl_MatrixGuard_hpp

#	include <stack>

#	include <GL/gl.h> // GLenum

#	include "../../util/class/special_member_functions.hpp" // Uncopyable

namespace page
{
	namespace vid
	{
		namespace opengl
		{
			struct MatrixGuard : util::Uncopyable<MatrixGuard>
			{
				// destroy
				~MatrixGuard();

				// modifiers
				void Push();
				void Pop();
				void Release();
				void Restore();

				private:
				struct State
				{
					GLenum mode, texture;
				};
				typedef std::stack<State> Stack;
				Stack stack;
			};
		}
	}
}

#endif
