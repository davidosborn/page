/**
 * @section license
 *
 * Copyright (c) 2006-2013 David Osborn
 *
 * Permission is granted to use and redistribute this software in source and
 * binary form, with or without modification, subject to the following
 * conditions:
 *
 * 1. Redistributions in source form must retain the above copyright notice,
 *    this list of conditions, and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions, and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution, and in the same
 *    place and form as other copyright, license, and disclaimer information.
 *
 * As a special exception, distributions of derivative works in binary form may
 * include an acknowledgement in place of the above copyright notice, this list
 * of conditions, and the following disclaimer in the documentation and/or other
 * materials provided with the distribution, and in the same place and form as
 * other acknowledgements, similar in substance to the following:
 *
 *    Portions of this software are based on the work of David Osborn.
 *
 * This software is provided "as is", without any express or implied warranty.
 * In no event will the authors be liable for any damages arising out of the use
 * of this software.
 */

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
