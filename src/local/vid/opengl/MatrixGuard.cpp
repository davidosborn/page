#include <cassert>

#include "ext.hpp" // ARB_multitexture
#include "get.hpp" // GetInteger
#include "matrix.hpp" // {Pop,Push}Matrix
#include "MatrixGuard.hpp"

namespace page
{
	namespace vid
	{
		namespace opengl
		{
			// destroy
			MatrixGuard::~MatrixGuard()
			{
				Restore();
			}

			// modifiers
			void MatrixGuard::Push()
			{
				PushMatrix();
				State state;
				// save matrix mode
				state.mode = GetInteger(GL_MATRIX_MODE);
				// save active texture
				if (state.mode == GL_TEXTURE && haveArbMultitexture)
					state.texture = GetInteger(GL_ACTIVE_TEXTURE_ARB);
				stack.push(state);
			}
			void MatrixGuard::Pop()
			{
				assert(!stack.empty());
				const State &state(stack.top());
				// restore matrix mode
				glMatrixMode(state.mode);
				// restore active texture
				if (state.mode == GL_TEXTURE && haveArbMultitexture)
					glActiveTextureARB(state.texture);
				PopMatrix();
				stack.pop();
			}
			void MatrixGuard::Release()
			{
				while (!stack.empty()) stack.pop();
			}
			void MatrixGuard::Restore()
			{
				while (!stack.empty()) Pop();
			}
		}
	}
}
