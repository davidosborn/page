#include <stack>
#include <unordered_map>

#include <GL/gl.h>

#include "../../math/Matrix.hpp"
#include "get.hpp" // GetInteger

namespace page
{
	namespace vid
	{
		namespace opengl
		{
			namespace
			{
				typedef std::stack<math::Matrix<4, 4, GLfloat>> Stack;
				typedef std::unordered_map<GLenum, Stack> Modes;
				Modes &GetModes()
				{
					static Modes modes;
					return modes;
				}
			}

			void PushMatrix()
			{
				GLint mode = GetInteger(GL_MATRIX_MODE);
				GLenum depthQuery, maxDepthQuery, matrixQuery;
				switch (mode)
				{
					case GL_MODELVIEW:
					depthQuery    = GL_MODELVIEW_STACK_DEPTH;
					maxDepthQuery = GL_MAX_MODELVIEW_STACK_DEPTH;
					matrixQuery   = GL_MODELVIEW_MATRIX;
					break;
					case GL_PROJECTION:
					depthQuery    = GL_PROJECTION_STACK_DEPTH;
					maxDepthQuery = GL_MAX_PROJECTION_STACK_DEPTH;
					matrixQuery   = GL_PROJECTION_MATRIX;
					break;
					case GL_TEXTURE:
					depthQuery    = GL_TEXTURE_STACK_DEPTH;
					maxDepthQuery = GL_MAX_TEXTURE_STACK_DEPTH;
					matrixQuery   = GL_TEXTURE_MATRIX;
					break;
					default: glPushMatrix(); return;
				}
				if (GetInteger(depthQuery) < GetInteger(maxDepthQuery))
				{
					glPushMatrix();
					return;
				}
				Stack &stack(GetModes()[mode]);
				math::Matrix<4, 4, GLfloat> matrix;
				glGetFloatv(matrixQuery, &*matrix.begin());
				stack.push(matrix);
			}
			void PopMatrix()
			{
				GLint mode = GetInteger(GL_MATRIX_MODE);
				Modes::iterator iter(GetModes().find(mode));
				if (iter == GetModes().end())
				{
					glPopMatrix();
					return;
				}
				Stack &stack(iter->second);
				if (stack.empty())
				{
					glPopMatrix();
					return;
				}
				math::Matrix<4, 4, GLfloat> matrix(stack.top());
				glLoadMatrixf(&*matrix.begin());
				stack.pop();
			}
		}
	}
}
