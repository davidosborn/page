/**
 * @section license
 *
 * Copyright (c) 2006-2012 David Osborn
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

#include <array>
#include <cassert>
#include <stack>
#include <vector>

#include <boost/optional.hpp>

#define GL_PURE
#include <GL/gl.h>

#define GLEXT_PURE
#include <GL/glext.h>

#ifdef HAVE_GLU
#	define GLU_PURE
#	include <GL/glu.h>
#endif

#include "ext.hpp" // ARB_{multitexture,shader_objects}, EXT_{blend_func_separate,framebuffer_object}

namespace page
{
	namespace vid
	{
		namespace opengl
		{
			namespace
			{
				struct State
				{
					State()
					{
						glGetBooleanv(GL_DEPTH_TEST, &depthTest);
						glGetIntegerv(GL_DRAW_BUFFER, reinterpret_cast<GLint *>(&drawBuffer));
						glGetIntegerv(GL_MATRIX_MODE, reinterpret_cast<GLint *>(&matrixMode));
						glGetIntegerv(GL_MAX_MODELVIEW_STACK_DEPTH, &maxModelviewStackDepth);
						glGetIntegerv(GL_MAX_PROJECTION_STACK_DEPTH, &maxProjectionStackDepth);
						glGetIntegerv(GL_MODELVIEW_STACK_DEPTH, &modelviewStackDepth);
						glGetIntegerv(GL_PROJECTION_STACK_DEPTH, &projectionStackDepth);
						glGetIntegerv(GL_SCISSOR_BOX, &*scissorBox.begin());
						glGetBooleanv(GL_SCISSOR_TEST, &scissorTest);
						glGetIntegerv(GL_VIEWPORT, &*viewport.begin());
						// initialize extension state
						// ARB_multitexture
						if (haveArbMultitexture)
						{
							glGetIntegerv(GL_ACTIVE_TEXTURE_ARB, reinterpret_cast<GLint *>(&activeTexture));
							glGetIntegerv(GL_CLIENT_ACTIVE_TEXTURE_ARB, reinterpret_cast<GLint *>(&clientActiveTexture));
							// initialize texture units
							GLint maxTextureUnits;
							glGetIntegerv(GL_MAX_TEXTURE_UNITS_ARB, &maxTextureUnits);
							textureUnits.reserve(maxTextureUnits);
							for (unsigned i = 0; i < maxTextureUnits; ++i)
							{
								glActiveTextureARB(GL_TEXTURE0_ARB + i);
								textureUnits.push_back(TextureUnit());
							}
							glActiveTextureARB(GL_TEXTURE0_ARB);
						}
						else textureUnits.push_back(TextureUnit());
						// ARB_shader_objects
						if (haveArbShaderObjects)
							programObject = glGetHandleARB(GL_PROGRAM_OBJECT_ARB);
						// EXT_blend_func_separate
						if (haveExtBlendFuncSeparate)
						{
							glGetIntegerv(GL_BLEND_DST_ALPHA_EXT, reinterpret_cast<GLint *>(&blendDstAlpha));
							glGetIntegerv(GL_BLEND_DST_RGB_EXT, reinterpret_cast<GLint *>(&blendDstRgb));
							glGetIntegerv(GL_BLEND_SRC_ALPHA_EXT, reinterpret_cast<GLint *>(&blendSrcAlpha));
							glGetIntegerv(GL_BLEND_SRC_RGB_EXT, reinterpret_cast<GLint *>(&blendSrcRgb));
						}
						else
						{
							glGetIntegerv(GL_BLEND_DST, reinterpret_cast<GLint *>(&blendDst));
							glGetIntegerv(GL_BLEND_SRC, reinterpret_cast<GLint *>(&blendSrc));
						}
						// EXT_framebuffer_object
						if (haveExtFramebufferObject)
							glGetIntegerv(GL_FRAMEBUFFER_BINDING_EXT, reinterpret_cast<GLint *>(&framebufferBinding));
					}

					GLenum               activeTexture;
					union
					{
						GLenum           blendDst;
						GLenum           blendDstRgb;
					};
					GLenum               blendDstAlpha;
					union
					{
						GLenum           blendSrc;
						GLenum           blendSrcRgb;
					};
					GLenum               blendSrcAlpha;
					GLenum               clientActiveTexture;
					GLboolean            depthTest;
					GLenum               drawBuffer;
					GLuint               framebufferBinding;
					GLenum               matrixMode;
					GLint                maxModelviewStackDepth;
					GLint                maxProjectionStackDepth;
					GLint                modelviewStackDepth;
					GLhandleARB          programObject;
					GLint                projectionStackDepth;
					std::array<GLint, 4> scissorBox;
					GLboolean            scissorTest;
					std::array<GLint, 4> viewport;

					struct Attrib
					{
						boost::optional<GLenum>               activeTexture;
						boost::optional<GLenum>               blendDstAlpha;
						boost::optional<GLenum>               blendDstRgb;
						boost::optional<GLenum>               blendSrcAlpha;
						boost::optional<GLenum>               blendSrcRgb;
						boost::optional<GLboolean>            depthTest;
						boost::optional<GLenum>               drawBuffer;
						boost::optional<GLenum>               matrixMode;
						boost::optional<std::array<GLint, 4>> scissorBox;
						boost::optional<GLboolean>            scissorTest;
						boost::optional<std::array<GLint, 4>> viewport;
					};
					typedef std::stack<Attrib> AttribStack;
					AttribStack attribStack;

					struct ClientAttrib
					{
						boost::optional<GLenum> clientActiveTexture;
					};
					typedef std::stack<ClientAttrib> ClientAttribStack;
					ClientAttribStack clientAttribStack;

					struct TextureUnit
					{
						TextureUnit()
						{
							glGetIntegerv(GL_MAX_TEXTURE_STACK_DEPTH, &maxTextureStackDepth);
							glGetIntegerv(GL_TEXTURE_STACK_DEPTH, &textureStackDepth);
						}

						GLint maxTextureStackDepth;
						GLint textureStackDepth;
					};
					typedef std::vector<TextureUnit> TextureUnits;
					TextureUnits textureUnits;
				};
				State &GetState()
				{
					static State state;
					return state;
				}

				// generic getter
				template <typename T> bool GetValue(GLenum value, T *data)
				{
					const State &state(GetState());
					switch (value)
					{
						case GL_ACTIVE_TEXTURE_ARB:         *data = state.activeTexture;           break;
						case GL_BLEND_DST:                  *data = state.blendDst;                break;
						case GL_BLEND_DST_ALPHA_EXT:        *data = state.blendDstAlpha;           break;
						case GL_BLEND_DST_RGB_EXT:          *data = state.blendDstRgb;             break;
						case GL_BLEND_SRC:                  *data = state.blendSrc;                break;
						case GL_BLEND_SRC_ALPHA_EXT:        *data = state.blendSrcAlpha;           break;
						case GL_BLEND_SRC_RGB_EXT:          *data = state.blendSrcRgb;             break;
						case GL_CLIENT_ACTIVE_TEXTURE_ARB:  *data = state.clientActiveTexture;     break;
						case GL_DEPTH_TEST:                 *data = state.depthTest;               break;
						case GL_DRAW_BUFFER:                *data = state.drawBuffer;              break;
						case GL_FRAMEBUFFER_BINDING_EXT:    *data = state.framebufferBinding;      break;
						case GL_MATRIX_MODE:                *data = state.matrixMode;              break;
						case GL_MAX_MODELVIEW_STACK_DEPTH:  *data = state.maxModelviewStackDepth;  break;
						case GL_MAX_PROJECTION_STACK_DEPTH: *data = state.maxProjectionStackDepth; break;
						case GL_MAX_TEXTURE_UNITS_ARB:      *data = state.textureUnits.size();     break;
						case GL_MODELVIEW_STACK_DEPTH:      *data = state.modelviewStackDepth;     break;
						case GL_PROJECTION_STACK_DEPTH:     *data = state.projectionStackDepth;    break;
						case GL_SCISSOR_BOX:                *data = state.scissorBox;              break;
						case GL_SCISSOR_TEST:               *data = state.scissorTest;             break;
						case GL_VIEWPORT:                   *data = state.viewport;                break;
						default: return false;
					}
					return true;
				}
			}

			// core functions
			void glBlendFunc_Shadow(GLenum src, GLenum dst)
			{
				State &state(GetState());
				state.blendSrcAlpha = state.blendSrcRgb = src;
				state.blendDstAlpha = state.blendDstRgb = dst;
				glBlendFunc(src, dst);
			}
			void glDisable_Shadow(GLenum target)
			{
				State &state(GetState());
				switch (target)
				{
					case GL_DEPTH_TEST:   state.depthTest   = GL_FALSE; break;
					case GL_SCISSOR_TEST: state.scissorTest = GL_FALSE; break;
				}
				glDisable(target);
			}
			void glDrawBuffer_Shadow(GLenum mode)
			{
				State &state(GetState());
				if (mode != state.drawBuffer)
				{
					state.drawBuffer = mode;
					glDrawBuffer(mode);
				}
			}
			void glEnable_Shadow(GLenum target)
			{
				State &state(GetState());
				switch (target)
				{
					case GL_DEPTH_TEST:   state.depthTest   = GL_TRUE; break;
					case GL_SCISSOR_TEST: state.scissorTest = GL_TRUE; break;
				}
				glEnable(target);
			}
			void glGetBooleanv_Shadow(GLenum value, GLboolean *data)
			{
				if (!GetValue(value, data)) glGetBooleanv(value, data);
			}
			void glGetDoublev_Shadow(GLenum value, GLdouble *data)
			{
				if (!GetValue(value, data)) glGetDoublev(value, data);
			}
			void glGetFloatv_Shadow(GLenum value, GLfloat *data)
			{
				if (!GetValue(value, data)) glGetFloatv(value, data);
			}
			void glGetIntegerv_Shadow(GLenum value, GLint *data)
			{
				if (!GetValue(value, data)) glGetIntegerv(value, data);
			}
			void glMatrixMode_Shadow(GLenum mode)
			{
				glMatrixMode(GetState().matrixMode = mode);
			}
			void glPopAttrib_Shadow()
			{
				State &state(GetState());
				assert(!state.attribStack.empty());
				const State::Attrib &attrib(state.attribStack.top());
				if (attrib.activeTexture) state.activeTexture = *attrib.activeTexture;
				if (attrib.blendDstAlpha) state.blendDstAlpha = *attrib.blendDstAlpha;
				if (attrib.blendDstRgb)   state.blendDstRgb   = *attrib.blendDstRgb;
				if (attrib.blendSrcAlpha) state.blendSrcAlpha = *attrib.blendSrcAlpha;
				if (attrib.blendSrcRgb)   state.blendSrcRgb   = *attrib.blendSrcRgb;
				if (attrib.depthTest)     state.depthTest     = *attrib.depthTest;
				if (attrib.drawBuffer)    state.drawBuffer    = *attrib.drawBuffer;
				if (attrib.matrixMode)    state.matrixMode    = *attrib.matrixMode;
				if (attrib.scissorBox)    state.scissorBox    = *attrib.scissorBox;
				if (attrib.scissorTest)   state.scissorTest   = *attrib.scissorTest;
				if (attrib.viewport)      state.viewport      = *attrib.viewport;
				state.attribStack.pop();
				glPopAttrib();
			}
			void glPopClientAttrib_Shadow()
			{
				State &state(GetState());
				assert(!state.clientAttribStack.empty());
				const State::ClientAttrib &clientAttrib(state.clientAttribStack.top());
				if (clientAttrib.clientActiveTexture) state.clientActiveTexture = *clientAttrib.clientActiveTexture;
				state.clientAttribStack.pop();
				glPopClientAttrib();
			}
			void glPopMatrix_Shadow()
			{
				State &state(GetState());
				switch (state.matrixMode)
				{
					case GL_MODELVIEW:  --state.modelviewStackDepth;  break;
					case GL_PROJECTION: --state.projectionStackDepth; break;
				}
				glPopMatrix();
			}
			void glPushAttrib_Shadow(GLbitfield mask)
			{
				State &state(GetState());
				State::Attrib attrib;
				if (mask & GL_COLOR_BUFFER_BIT)
				{
					attrib.blendDstAlpha = state.blendDstAlpha;
					attrib.blendDstRgb = state.blendDstRgb;
					attrib.blendSrcAlpha = state.blendSrcAlpha;
					attrib.blendSrcRgb = state.blendSrcRgb;
					attrib.drawBuffer = state.drawBuffer;
				}
				if (mask & GL_DEPTH_BUFFER_BIT)
				{
					attrib.depthTest = state.depthTest;
				}
				if (mask & GL_ENABLE_BIT)
				{
					attrib.depthTest = state.depthTest;
					attrib.scissorTest = state.scissorTest;
				}
				if (mask & GL_SCISSOR_BIT)
				{
					attrib.scissorBox = state.scissorBox;
					attrib.scissorTest = state.scissorTest;
				}
				if (mask & GL_TEXTURE_BIT)
				{
					attrib.activeTexture = state.activeTexture;
				}
				if (mask & GL_TRANSFORM_BIT)
				{
					attrib.matrixMode = state.matrixMode;
				}
				if (mask & GL_VIEWPORT_BIT)
				{
					attrib.viewport = state.viewport;
				}
				state.attribStack.push(attrib);
				glPushAttrib(mask);
			}
			void glPushClientAttrib_Shadow(GLbitfield mask)
			{
				State &state(GetState());
				State::ClientAttrib clientAttrib;
				if (mask & GL_CLIENT_VERTEX_ARRAY_BIT)
				{
					clientAttrib.clientActiveTexture = state.clientActiveTexture;
				}
				state.clientAttribStack.push(clientAttrib);
				glPushClientAttrib(mask);
			}
			void glPushMatrix_Shadow()
			{
				State &state(GetState());
				switch (state.matrixMode)
				{
					case GL_MODELVIEW:  ++state.modelviewStackDepth;  break;
					case GL_PROJECTION: ++state.projectionStackDepth; break;
				}
				glPushMatrix();
			}
			void glScissor_Shadow(GLint x, GLint y, GLsizei width, GLsizei height)
			{
				State &state(GetState());
				state.scissorBox[0] = x;
				state.scissorBox[1] = y;
				state.scissorBox[2] = width;
				state.scissorBox[3] = height;
				glScissor(x, y, width, height);
			}
			void glViewport_Shadow(GLint x, GLint y, GLsizei width, GLsizei height)
			{
				State &state(GetState());
				state.viewport[0] = x;
				state.viewport[1] = y;
				state.viewport[2] = width;
				state.viewport[3] = height;
				glViewport(x, y, width, height);
			}

#ifdef HAVE_GLU
			// utility functions
			void gluLookAt_Shadow(GLdouble eyex, GLdouble eyey, GLdouble eyez, GLdouble centerx, GLdouble centery, GLdouble centerz, GLdouble upx, GLdouble upy, GLdouble upz)
			{
				// FIXME: implement
				gluLookAt(eyex, eyey, eyez, centerx, centery, centerz, upx, upy, upz);
			}
			void gluOrtho2D_Shadow(GLdouble left, GLdouble right, GLdouble bottom, GLdouble far)
			{
				// FIXME: implement
				gluOrtho2D(left, right, bottom, far);
			}
			void gluPerspective_Shadow(GLdouble fovy, GLdouble aspect, GLdouble near, GLdouble far)
			{
				// FIXME: implement
				gluPerspective(fovy, aspect, near, far);
			}
			void gluPickMatrix_Shadow(GLdouble x, GLdouble y, GLdouble width, GLdouble height, GLint viewport[4])
			{
				// FIXME: implement
				gluPickMatrix(x, y, width, height, viewport);
			}
			int gluProject_Shadow(GLdouble objx, GLdouble objy, GLdouble objz, const GLdouble modelMatrix[16], const GLdouble projMatrix[16], const GLint viewport[4], GLdouble *winx, GLdouble *winy, GLdouble *winz)
			{
				// FIXME: implement
				return gluProject(objx, objy, objz, modelMatrix, projMatrix, viewport, winx, winy, winz);
			}
			int gluUnProject_Shadow(GLdouble winx, GLdouble winy, GLdouble winz, const GLdouble modelMatrix[16], const GLdouble projMatrix[16], const GLint viewport[4], GLdouble *objx, GLdouble *objy, GLdouble *objz)
			{
				// FIXME: implement
				return gluUnProject(winx, winy, winz, modelMatrix, projMatrix, viewport, objx, objy, objz);
			}
#endif

			// extension functions
			// ARB_multitexture
			void glActiveTextureARB_Shadow(GLenum texture)
			{
				GetState().activeTexture = texture;
				glActiveTextureARB(texture);
			}
			void glClientActiveTextureARB_Shadow(GLenum texture)
			{
				GetState().clientActiveTexture = texture;
				glClientActiveTextureARB(texture);
			}
			// ARB_shader_objects
			GLhandleARB glGetHandleARB_Shadow(GLenum pname)
			{
				const State &state(GetState());
				switch (pname)
				{
					case GL_PROGRAM_OBJECT_ARB: return state.programObject;
				}
				return glGetHandleARB(pname);
			}
			void glUseProgramObjectARB_Shadow(GLhandleARB programObj)
			{
				GetState().programObject = programObj;
				glUseProgramObjectARB(programObj);
			}
			// EXT_blend_func_separate
			void glBlendFuncSeparateEXT_Shadow(GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha)
			{
				State &state(GetState());
				state.blendSrcRgb = srcRGB;
				state.blendDstRgb = dstRGB;
				state.blendSrcAlpha = srcAlpha;
				state.blendDstAlpha = dstAlpha;
				glBlendFuncSeparateEXT(srcRGB, dstRGB, srcAlpha, dstAlpha);
			}
			// EXT_framebuffer_object
			void glBindFramebufferEXT_Shadow(GLenum target, GLuint framebuffer)
			{
				State &state(GetState());
				switch (target)
				{
					case GL_FRAMEBUFFER_EXT:
					if (framebuffer == state.framebufferBinding) return;
					state.framebufferBinding = framebuffer;
					break;
				}
				glBindFramebufferEXT(target, framebuffer);
			}
		}
	}
}
