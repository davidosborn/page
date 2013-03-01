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

#include <algorithm> // max
#include <array>
#include <iomanip> // setw
#include <iostream> // cout
#include <iterator> // inserter
#include <unordered_set>
#include <vector>

#include <boost/io/ios_state.hpp> // ios_all_saver

#define GLEXT_PURE
#include <GL/glext.h>

#include "../../cfg/vars.hpp"
#include "../../log/Indenter.hpp"
#include "../../util/io/deserialize.hpp" // Deserialize
#include "../../util/cpp.hpp" // STRINGIZE
#include "ext.hpp" // GetProcAddress, ProcAddress

namespace page
{
	namespace vid
	{
		namespace opengl
		{
			bool
				haveArbColorBufferFloat,
				haveArbDepthTexture,
				haveArbDrawBuffers,
				haveArbFragmentShader,
				haveArbHalfFloatPixel,
				haveArbMultitexture,
				haveArbPointSprite,
				haveArbPointParameters,
				haveArbShaderObjects,
				haveArbShadingLanguage100,
				haveArbShadow,
				haveArbTextureBorderClamp,
				haveArbTextureCubeMap,
				haveArbTextureEnvAdd,
				haveArbTextureEnvCombine,
				haveArbTextureEnvDot3,
				haveArbTextureFloat,
				haveArbTextureNonPowerOfTwo,
				haveArbTextureRectangle,
				haveArbVertexBufferObject,
				haveArbVertexShader,
				haveAtiTextureFloat,
				haveExtAbgr,
				haveExtBgra,
				haveExtBlendFuncSeparate,
				haveExtBlendMinmax,
				haveExtBlendSubtract,
				haveExtFramebufferObject,
				haveExtTexture3d,
				haveExtTextureEdgeClamp,
				haveNvFloatBuffer,
				haveNvRegisterCombiners,
				haveNvTextureShader,
				haveSgisGenerateMipmap,
				haveSgisTextureEdgeClamp;

			// ARB_color_buffer_float
			PFNGLCLAMPCOLORARBPROC                          glClampColorARB;
			// ARB_depth_texture
			// ARB_draw_buffers
			PFNGLDRAWBUFFERSARBPROC                         glDrawBuffersARB;
			// ARB_fragment_shader
			// ARB_half_float_pixel
			// ARB_multitexture
			PFNGLACTIVETEXTUREARBPROC                       glActiveTextureARB;
			PFNGLCLIENTACTIVETEXTUREARBPROC                 glClientActiveTextureARB;
			PFNGLMULTITEXCOORD1DARBPROC                     glMultiTexCoord1dARB;
			PFNGLMULTITEXCOORD1DVARBPROC                    glMultiTexCoord1dvARB;
			PFNGLMULTITEXCOORD1FARBPROC                     glMultiTexCoord1fARB;
			PFNGLMULTITEXCOORD1FVARBPROC                    glMultiTexCoord1fvARB;
			PFNGLMULTITEXCOORD1IARBPROC                     glMultiTexCoord1iARB;
			PFNGLMULTITEXCOORD1IVARBPROC                    glMultiTexCoord1ivARB;
			PFNGLMULTITEXCOORD1SARBPROC                     glMultiTexCoord1sARB;
			PFNGLMULTITEXCOORD1SVARBPROC                    glMultiTexCoord1svARB;
			PFNGLMULTITEXCOORD2DARBPROC                     glMultiTexCoord2dARB;
			PFNGLMULTITEXCOORD2DVARBPROC                    glMultiTexCoord2dvARB;
			PFNGLMULTITEXCOORD2FARBPROC                     glMultiTexCoord2fARB;
			PFNGLMULTITEXCOORD2FVARBPROC                    glMultiTexCoord2fvARB;
			PFNGLMULTITEXCOORD2IARBPROC                     glMultiTexCoord2iARB;
			PFNGLMULTITEXCOORD2IVARBPROC                    glMultiTexCoord2ivARB;
			PFNGLMULTITEXCOORD2SARBPROC                     glMultiTexCoord2sARB;
			PFNGLMULTITEXCOORD2SVARBPROC                    glMultiTexCoord2svARB;
			PFNGLMULTITEXCOORD3DARBPROC                     glMultiTexCoord3dARB;
			PFNGLMULTITEXCOORD3DVARBPROC                    glMultiTexCoord3dvARB;
			PFNGLMULTITEXCOORD3FARBPROC                     glMultiTexCoord3fARB;
			PFNGLMULTITEXCOORD3FVARBPROC                    glMultiTexCoord3fvARB;
			PFNGLMULTITEXCOORD3IARBPROC                     glMultiTexCoord3iARB;
			PFNGLMULTITEXCOORD3IVARBPROC                    glMultiTexCoord3ivARB;
			PFNGLMULTITEXCOORD3SARBPROC                     glMultiTexCoord3sARB;
			PFNGLMULTITEXCOORD3SVARBPROC                    glMultiTexCoord3svARB;
			PFNGLMULTITEXCOORD4DARBPROC                     glMultiTexCoord4dARB;
			PFNGLMULTITEXCOORD4DVARBPROC                    glMultiTexCoord4dvARB;
			PFNGLMULTITEXCOORD4FARBPROC                     glMultiTexCoord4fARB;
			PFNGLMULTITEXCOORD4FVARBPROC                    glMultiTexCoord4fvARB;
			PFNGLMULTITEXCOORD4IARBPROC                     glMultiTexCoord4iARB;
			PFNGLMULTITEXCOORD4IVARBPROC                    glMultiTexCoord4ivARB;
			PFNGLMULTITEXCOORD4SARBPROC                     glMultiTexCoord4sARB;
			PFNGLMULTITEXCOORD4SVARBPROC                    glMultiTexCoord4svARB;
			// ARB_point_sprite
			// ARB_point_parameters
			PFNGLPOINTPARAMETERFARBPROC                     glPointParameterfARB;
			PFNGLPOINTPARAMETERFVARBPROC                    glPointParameterfvARB;
			// ARB_shader_objects
			PFNGLDELETEOBJECTARBPROC                        glDeleteObjectARB;
			PFNGLGETHANDLEARBPROC                           glGetHandleARB;
			PFNGLDETACHOBJECTARBPROC                        glDetachObjectARB;
			PFNGLCREATESHADEROBJECTARBPROC                  glCreateShaderObjectARB;
			PFNGLSHADERSOURCEARBPROC                        glShaderSourceARB;
			PFNGLCOMPILESHADERARBPROC                       glCompileShaderARB;
			PFNGLCREATEPROGRAMOBJECTARBPROC                 glCreateProgramObjectARB;
			PFNGLATTACHOBJECTARBPROC                        glAttachObjectARB;
			PFNGLLINKPROGRAMARBPROC                         glLinkProgramARB;
			PFNGLUSEPROGRAMOBJECTARBPROC                    glUseProgramObjectARB;
			PFNGLVALIDATEPROGRAMARBPROC                     glValidateProgramARB;
			PFNGLUNIFORM1FARBPROC                           glUniform1fARB;
			PFNGLUNIFORM2FARBPROC                           glUniform2fARB;
			PFNGLUNIFORM3FARBPROC                           glUniform3fARB;
			PFNGLUNIFORM4FARBPROC                           glUniform4fARB;
			PFNGLUNIFORM1IARBPROC                           glUniform1iARB;
			PFNGLUNIFORM2IARBPROC                           glUniform2iARB;
			PFNGLUNIFORM3IARBPROC                           glUniform3iARB;
			PFNGLUNIFORM4IARBPROC                           glUniform4iARB;
			PFNGLUNIFORM1FVARBPROC                          glUniform1fvARB;
			PFNGLUNIFORM2FVARBPROC                          glUniform2fvARB;
			PFNGLUNIFORM3FVARBPROC                          glUniform3fvARB;
			PFNGLUNIFORM4FVARBPROC                          glUniform4fvARB;
			PFNGLUNIFORM1IVARBPROC                          glUniform1ivARB;
			PFNGLUNIFORM2IVARBPROC                          glUniform2ivARB;
			PFNGLUNIFORM3IVARBPROC                          glUniform3ivARB;
			PFNGLUNIFORM4IVARBPROC                          glUniform4ivARB;
			PFNGLUNIFORMMATRIX2FVARBPROC                    glUniformMatrix2fvARB;
			PFNGLUNIFORMMATRIX3FVARBPROC                    glUniformMatrix3fvARB;
			PFNGLUNIFORMMATRIX4FVARBPROC                    glUniformMatrix4fvARB;
			PFNGLGETOBJECTPARAMETERFVARBPROC                glGetObjectParameterfvARB;
			PFNGLGETOBJECTPARAMETERIVARBPROC                glGetObjectParameterivARB;
			PFNGLGETINFOLOGARBPROC                          glGetInfoLogARB;
			PFNGLGETATTACHEDOBJECTSARBPROC                  glGetAttachedObjectsARB;
			PFNGLGETUNIFORMLOCATIONARBPROC                  glGetUniformLocationARB;
			PFNGLGETACTIVEUNIFORMARBPROC                    glGetActiveUniformARB;
			PFNGLGETUNIFORMFVARBPROC                        glGetUniformfvARB;
			PFNGLGETUNIFORMIVARBPROC                        glGetUniformivARB;
			PFNGLGETSHADERSOURCEARBPROC                     glGetShaderSourceARB;
			// ARB_shading_language_100
			// ARB_shadow
			// ARB_texture_border_clamp
			// ARB_texture_cube_map
			// ARB_texture_env_add
			// ARB_texture_env_combine
			// ARB_texture_env_dot3
			// ARB_texture_float
			// ARB_texture_non_power_of_two
			// ARB_texture_rectangle
			// ARB_vertex_buffer_object
			PFNGLBINDBUFFERARBPROC                          glBindBufferARB;
			PFNGLDELETEBUFFERSARBPROC                       glDeleteBuffersARB;
			PFNGLGENBUFFERSARBPROC                          glGenBuffersARB;
			PFNGLISBUFFERARBPROC                            glIsBufferARB;
			PFNGLBUFFERDATAARBPROC                          glBufferDataARB;
			PFNGLBUFFERSUBDATAARBPROC                       glBufferSubDataARB;
			PFNGLGETBUFFERSUBDATAARBPROC                    glGetBufferSubDataARB;
			PFNGLMAPBUFFERARBPROC                           glMapBufferARB;
			PFNGLUNMAPBUFFERARBPROC                         glUnmapBufferARB;
			PFNGLGETBUFFERPARAMETERIVARBPROC                glGetBufferParameterivARB;
			PFNGLGETBUFFERPOINTERVARBPROC                   glGetBufferPointervARB;
			// ARB_vertex_shader
			PFNGLVERTEXATTRIB1FARBPROC                      glVertexAttrib1fARB;
			PFNGLVERTEXATTRIB1SARBPROC                      glVertexAttrib1sARB;
			PFNGLVERTEXATTRIB1DARBPROC                      glVertexAttrib1dARB;
			PFNGLVERTEXATTRIB2FARBPROC                      glVertexAttrib2fARB;
			PFNGLVERTEXATTRIB2SARBPROC                      glVertexAttrib2sARB;
			PFNGLVERTEXATTRIB2DARBPROC                      glVertexAttrib2dARB;
			PFNGLVERTEXATTRIB3FARBPROC                      glVertexAttrib3fARB;
			PFNGLVERTEXATTRIB3SARBPROC                      glVertexAttrib3sARB;
			PFNGLVERTEXATTRIB3DARBPROC                      glVertexAttrib3dARB;
			PFNGLVERTEXATTRIB4FARBPROC                      glVertexAttrib4fARB;
			PFNGLVERTEXATTRIB4SARBPROC                      glVertexAttrib4sARB;
			PFNGLVERTEXATTRIB4DARBPROC                      glVertexAttrib4dARB;
			PFNGLVERTEXATTRIB4NUBARBPROC                    glVertexAttrib4NubARB;
			PFNGLVERTEXATTRIB1FVARBPROC                     glVertexAttrib1fvARB;
			PFNGLVERTEXATTRIB1SVARBPROC                     glVertexAttrib1svARB;
			PFNGLVERTEXATTRIB1DVARBPROC                     glVertexAttrib1dvARB;
			PFNGLVERTEXATTRIB2FVARBPROC                     glVertexAttrib2fvARB;
			PFNGLVERTEXATTRIB2SVARBPROC                     glVertexAttrib2svARB;
			PFNGLVERTEXATTRIB2DVARBPROC                     glVertexAttrib2dvARB;
			PFNGLVERTEXATTRIB3FVARBPROC                     glVertexAttrib3fvARB;
			PFNGLVERTEXATTRIB3SVARBPROC                     glVertexAttrib3svARB;
			PFNGLVERTEXATTRIB3DVARBPROC                     glVertexAttrib3dvARB;
			PFNGLVERTEXATTRIB4FVARBPROC                     glVertexAttrib4fvARB;
			PFNGLVERTEXATTRIB4SVARBPROC                     glVertexAttrib4svARB;
			PFNGLVERTEXATTRIB4DVARBPROC                     glVertexAttrib4dvARB;
			PFNGLVERTEXATTRIB4IVARBPROC                     glVertexAttrib4ivARB;
			PFNGLVERTEXATTRIB4BVARBPROC                     glVertexAttrib4bvARB;
			PFNGLVERTEXATTRIB4UBVARBPROC                    glVertexAttrib4ubvARB;
			PFNGLVERTEXATTRIB4USVARBPROC                    glVertexAttrib4usvARB;
			PFNGLVERTEXATTRIB4UIVARBPROC                    glVertexAttrib4uivARB;
			PFNGLVERTEXATTRIB4NBVARBPROC                    glVertexAttrib4NbvARB;
			PFNGLVERTEXATTRIB4NSVARBPROC                    glVertexAttrib4NsvARB;
			PFNGLVERTEXATTRIB4NIVARBPROC                    glVertexAttrib4NivARB;
			PFNGLVERTEXATTRIB4NUBVARBPROC                   glVertexAttrib4NubvARB;
			PFNGLVERTEXATTRIB4NUSVARBPROC                   glVertexAttrib4NusvARB;
			PFNGLVERTEXATTRIB4NUIVARBPROC                   glVertexAttrib4NuivARB;
			PFNGLVERTEXATTRIBPOINTERARBPROC                 glVertexAttribPointerARB;
			PFNGLENABLEVERTEXATTRIBARRAYARBPROC             glEnableVertexAttribArrayARB;
			PFNGLDISABLEVERTEXATTRIBARRAYARBPROC            glDisableVertexAttribArrayARB;
			PFNGLBINDATTRIBLOCATIONARBPROC                  glBindAttribLocationARB;
			PFNGLGETACTIVEATTRIBARBPROC                     glGetActiveAttribARB;
			PFNGLGETATTRIBLOCATIONARBPROC                   glGetAttribLocationARB;
			PFNGLGETVERTEXATTRIBDVARBPROC                   glGetVertexAttribdvARB;
			PFNGLGETVERTEXATTRIBFVARBPROC                   glGetVertexAttribfvARB;
			PFNGLGETVERTEXATTRIBIVARBPROC                   glGetVertexAttribivARB;
			PFNGLGETVERTEXATTRIBPOINTERVARBPROC             glGetVertexAttribPointervARB;
			// ATI_texture_float
			// EXT_abgr
			// EXT_bgra
			// EXT_blend_func_separate
			PFNGLBLENDFUNCSEPARATEEXTPROC                   glBlendFuncSeparateEXT;
			// EXT_blend_minmax
			PFNGLBLENDEQUATIONEXTPROC                       glBlendEquationEXT;
			// EXT_blend_subtract
			// EXT_framebuffer_object
			PFNGLISRENDERBUFFEREXTPROC                      glIsRenderbufferEXT;
			PFNGLBINDRENDERBUFFEREXTPROC                    glBindRenderbufferEXT;
			PFNGLDELETERENDERBUFFERSEXTPROC                 glDeleteRenderbuffersEXT;
			PFNGLGENRENDERBUFFERSEXTPROC                    glGenRenderbuffersEXT;
			PFNGLRENDERBUFFERSTORAGEEXTPROC                 glRenderbufferStorageEXT;
			PFNGLGETRENDERBUFFERPARAMETERIVEXTPROC          glGetRenderbufferParameterivEXT;
			PFNGLISFRAMEBUFFEREXTPROC                       glIsFramebufferEXT;
			PFNGLBINDFRAMEBUFFEREXTPROC                     glBindFramebufferEXT;
			PFNGLDELETEFRAMEBUFFERSEXTPROC                  glDeleteFramebuffersEXT;
			PFNGLGENFRAMEBUFFERSEXTPROC                     glGenFramebuffersEXT;
			PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC              glCheckFramebufferStatusEXT;
			PFNGLFRAMEBUFFERTEXTURE1DEXTPROC                glFramebufferTexture1DEXT;
			PFNGLFRAMEBUFFERTEXTURE2DEXTPROC                glFramebufferTexture2DEXT;
			PFNGLFRAMEBUFFERTEXTURE3DEXTPROC                glFramebufferTexture3DEXT;
			PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC             glFramebufferRenderbufferEXT;
			PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVEXTPROC glGetFramebufferAttachmentParameterivEXT;
			PFNGLGENERATEMIPMAPEXTPROC                      glGenerateMipmapEXT;
			// EXT_texture3D
			PFNGLTEXIMAGE3DEXTPROC                          glTexImage3DEXT;
			// EXT_texture_edge_clamp
			// NV_float_buffer
			// NV_register_combiners
			PFNGLCOMBINERPARAMETERFVNVPROC                  glCombinerParameterfvNV;
			PFNGLCOMBINERPARAMETERIVNVPROC                  glCombinerParameterivNV;
			PFNGLCOMBINERPARAMETERFNVPROC                   glCombinerParameterfNV;
			PFNGLCOMBINERPARAMETERINVPROC                   glCombinerParameteriNV;
			PFNGLCOMBINERINPUTNVPROC                        glCombinerInputNV;
			PFNGLCOMBINEROUTPUTNVPROC                       glCombinerOutputNV;
			PFNGLFINALCOMBINERINPUTNVPROC                   glFinalCombinerInputNV;
			PFNGLGETCOMBINERINPUTPARAMETERFVNVPROC          glGetCombinerInputParameterfvNV;
			PFNGLGETCOMBINERINPUTPARAMETERIVNVPROC          glGetCombinerInputParameterivNV;
			PFNGLGETCOMBINEROUTPUTPARAMETERFVNVPROC         glGetCombinerOutputParameterfvNV;
			PFNGLGETCOMBINEROUTPUTPARAMETERIVNVPROC         glGetCombinerOutputParameterivNV;
			PFNGLGETFINALCOMBINERINPUTPARAMETERFVNVPROC     glGetFinalCombinerInputParameterfvNV;
			PFNGLGETFINALCOMBINERINPUTPARAMETERIVNVPROC     glGetFinalCombinerInputParameterivNV;
			// NV_texture_shader
			// SGIS_generate_mipmap
			// SGIS_texture_edge_clamp

			namespace
			{
#define PROC_ADDRESS(a) reinterpret_cast<ProcAddress *>(&a)
#define PROC(a) STRINGIZE(a), PROC_ADDRESS(a)

				struct Proc
				{
					std::string name;
					ProcAddress *address;
				};
				const std::array<Proc, 34> arbMultitextureProcs =
				{
					PROC(glActiveTextureARB),
					PROC(glClientActiveTextureARB),
					PROC(glMultiTexCoord1dARB),
					PROC(glMultiTexCoord1dvARB),
					PROC(glMultiTexCoord1fARB),
					PROC(glMultiTexCoord1fvARB),
					PROC(glMultiTexCoord1iARB),
					PROC(glMultiTexCoord1ivARB),
					PROC(glMultiTexCoord1sARB),
					PROC(glMultiTexCoord1svARB),
					PROC(glMultiTexCoord2dARB),
					PROC(glMultiTexCoord2dvARB),
					PROC(glMultiTexCoord2fARB),
					PROC(glMultiTexCoord2fvARB),
					PROC(glMultiTexCoord2iARB),
					PROC(glMultiTexCoord2ivARB),
					PROC(glMultiTexCoord2sARB),
					PROC(glMultiTexCoord2svARB),
					PROC(glMultiTexCoord3dARB),
					PROC(glMultiTexCoord3dvARB),
					PROC(glMultiTexCoord3fARB),
					PROC(glMultiTexCoord3fvARB),
					PROC(glMultiTexCoord3iARB),
					PROC(glMultiTexCoord3ivARB),
					PROC(glMultiTexCoord3sARB),
					PROC(glMultiTexCoord3svARB),
					PROC(glMultiTexCoord4dARB),
					PROC(glMultiTexCoord4dvARB),
					PROC(glMultiTexCoord4fARB),
					PROC(glMultiTexCoord4fvARB),
					PROC(glMultiTexCoord4iARB),
					PROC(glMultiTexCoord4ivARB),
					PROC(glMultiTexCoord4sARB),
					PROC(glMultiTexCoord4svARB)
				};
				const std::array<Proc, 2> arbPointParametersProcs =
				{
					PROC(glPointParameterfARB),
					PROC(glPointParameterfvARB)
				};
				const std::array<Proc, 39> arbShaderObjectsProcs =
				{
					PROC(glDeleteObjectARB),
					PROC(glGetHandleARB),
					PROC(glDetachObjectARB),
					PROC(glCreateShaderObjectARB),
					PROC(glShaderSourceARB),
					PROC(glCompileShaderARB),
					PROC(glCreateProgramObjectARB),
					PROC(glAttachObjectARB),
					PROC(glLinkProgramARB),
					PROC(glUseProgramObjectARB),
					PROC(glValidateProgramARB),
					PROC(glUniform1fARB),
					PROC(glUniform2fARB),
					PROC(glUniform3fARB),
					PROC(glUniform4fARB),
					PROC(glUniform1iARB),
					PROC(glUniform2iARB),
					PROC(glUniform3iARB),
					PROC(glUniform4iARB),
					PROC(glUniform1fvARB),
					PROC(glUniform2fvARB),
					PROC(glUniform3fvARB),
					PROC(glUniform4fvARB),
					PROC(glUniform1ivARB),
					PROC(glUniform2ivARB),
					PROC(glUniform3ivARB),
					PROC(glUniform4ivARB),
					PROC(glUniformMatrix2fvARB),
					PROC(glUniformMatrix3fvARB),
					PROC(glUniformMatrix4fvARB),
					PROC(glGetObjectParameterfvARB),
					PROC(glGetObjectParameterivARB),
					PROC(glGetInfoLogARB),
					PROC(glGetAttachedObjectsARB),
					PROC(glGetUniformLocationARB),
					PROC(glGetActiveUniformARB),
					PROC(glGetUniformfvARB),
					PROC(glGetUniformivARB),
					PROC(glGetShaderSourceARB)
				};
				const std::array<Proc, 11> arbVertexBufferObjectProcs =
				{
					PROC(glBindBufferARB),
					PROC(glDeleteBuffersARB),
					PROC(glGenBuffersARB),
					PROC(glIsBufferARB),
					PROC(glBufferDataARB),
					PROC(glBufferSubDataARB),
					PROC(glGetBufferSubDataARB),
					PROC(glMapBufferARB),
					PROC(glUnmapBufferARB),
					PROC(glGetBufferParameterivARB),
					PROC(glGetBufferPointervARB)
				};
				const std::array<Proc, 46> arbVertexShaderProcs =
				{
					PROC(glVertexAttrib1fARB),
					PROC(glVertexAttrib1sARB),
					PROC(glVertexAttrib1dARB),
					PROC(glVertexAttrib2fARB),
					PROC(glVertexAttrib2sARB),
					PROC(glVertexAttrib2dARB),
					PROC(glVertexAttrib3fARB),
					PROC(glVertexAttrib3sARB),
					PROC(glVertexAttrib3dARB),
					PROC(glVertexAttrib4fARB),
					PROC(glVertexAttrib4sARB),
					PROC(glVertexAttrib4dARB),
					PROC(glVertexAttrib4NubARB),
					PROC(glVertexAttrib1fvARB),
					PROC(glVertexAttrib1svARB),
					PROC(glVertexAttrib1dvARB),
					PROC(glVertexAttrib2fvARB),
					PROC(glVertexAttrib2svARB),
					PROC(glVertexAttrib2dvARB),
					PROC(glVertexAttrib3fvARB),
					PROC(glVertexAttrib3svARB),
					PROC(glVertexAttrib3dvARB),
					PROC(glVertexAttrib4fvARB),
					PROC(glVertexAttrib4svARB),
					PROC(glVertexAttrib4dvARB),
					PROC(glVertexAttrib4ivARB),
					PROC(glVertexAttrib4bvARB),
					PROC(glVertexAttrib4ubvARB),
					PROC(glVertexAttrib4usvARB),
					PROC(glVertexAttrib4uivARB),
					PROC(glVertexAttrib4NbvARB),
					PROC(glVertexAttrib4NsvARB),
					PROC(glVertexAttrib4NivARB),
					PROC(glVertexAttrib4NubvARB),
					PROC(glVertexAttrib4NusvARB),
					PROC(glVertexAttrib4NuivARB),
					PROC(glVertexAttribPointerARB),
					PROC(glEnableVertexAttribArrayARB),
					PROC(glDisableVertexAttribArrayARB),
					PROC(glBindAttribLocationARB),
					PROC(glGetActiveAttribARB),
					PROC(glGetAttribLocationARB),
					PROC(glGetVertexAttribdvARB),
					PROC(glGetVertexAttribfvARB),
					PROC(glGetVertexAttribivARB),
					PROC(glGetVertexAttribPointervARB)
				};
				const std::array<Proc, 1> extBlendFuncSeparateProcs =
				{
					PROC(glBlendFuncSeparateEXT)
				};
				const std::array<Proc, 1> extBlendMinmaxProcs =
				{
					PROC(glBlendEquationEXT)
				};
				const std::array<Proc, 17> extFramebufferObjectProcs =
				{
					PROC(glIsRenderbufferEXT),
					PROC(glBindRenderbufferEXT),
					PROC(glDeleteRenderbuffersEXT),
					PROC(glGenRenderbuffersEXT),
					PROC(glRenderbufferStorageEXT),
					PROC(glGetRenderbufferParameterivEXT),
					PROC(glIsFramebufferEXT),
					PROC(glBindFramebufferEXT),
					PROC(glDeleteFramebuffersEXT),
					PROC(glGenFramebuffersEXT),
					PROC(glCheckFramebufferStatusEXT),
					PROC(glFramebufferTexture1DEXT),
					PROC(glFramebufferTexture2DEXT),
					PROC(glFramebufferTexture3DEXT),
					PROC(glFramebufferRenderbufferEXT),
					PROC(glGetFramebufferAttachmentParameterivEXT),
					PROC(glGenerateMipmapEXT)
				};
				const std::array<Proc, 1> extTexture3dProcs =
				{
					PROC(glTexImage3DEXT)
				};
				const std::array<Proc, 13> nvRegisterCombinersProcs =
				{
					PROC(glCombinerParameterfvNV),
					PROC(glCombinerParameterivNV),
					PROC(glCombinerParameterfNV),
					PROC(glCombinerParameteriNV),
					PROC(glCombinerInputNV),
					PROC(glCombinerOutputNV),
					PROC(glFinalCombinerInputNV),
					PROC(glGetCombinerInputParameterfvNV),
					PROC(glGetCombinerInputParameterivNV),
					PROC(glGetCombinerOutputParameterfvNV),
					PROC(glGetCombinerOutputParameterivNV),
					PROC(glGetFinalCombinerInputParameterfvNV),
					PROC(glGetFinalCombinerInputParameterivNV)
				};

#define PROCS(a) Ext::Procs(a.begin(), a.end())
#define NO_PROCS Ext::Procs()

				struct Ext
				{
					const std::string name;
					bool &have;
					typedef std::vector<Proc> Procs;
					Procs procs;
				};
				typedef std::array<Ext, 35> Exts;
				Exts exts =
				{
					"GL_ARB_color_buffer_float",       haveArbColorBufferFloat,     NO_PROCS,
					"GL_ARB_depth_texture",            haveArbDepthTexture,         NO_PROCS,
					"GL_ARB_draw_buffers",             haveArbDrawBuffers,          NO_PROCS,
					"GL_ARB_fragment_shader",          haveArbFragmentShader,       NO_PROCS,
					"GL_ARB_half_float_pixel",         haveArbHalfFloatPixel,       NO_PROCS,
					"GL_ARB_multitexture",             haveArbMultitexture,         PROCS(arbMultitextureProcs),
					"GL_ARB_point_sprite",             haveArbPointSprite,          NO_PROCS,
					"GL_ARB_point_parameters",         haveArbPointParameters,      PROCS(arbPointParametersProcs),
					"GL_ARB_shader_objects",           haveArbShaderObjects,        PROCS(arbShaderObjectsProcs),
					"GL_ARB_shading_language_100",     haveArbShadingLanguage100,   NO_PROCS,
					"GL_ARB_shadow",                   haveArbShadow,               NO_PROCS,
					"GL_ARB_texture_border_clamp",     haveArbTextureBorderClamp,   NO_PROCS,
					"GL_ARB_texture_cube_map",         haveArbTextureCubeMap,       NO_PROCS,
					"GL_ARB_texture_env_add",          haveArbTextureEnvAdd,        NO_PROCS,
					"GL_ARB_texture_env_combine",      haveArbTextureEnvCombine,    NO_PROCS,
					"GL_ARB_texture_env_dot3",         haveArbTextureEnvDot3,       NO_PROCS,
					"GL_ARB_texture_float",            haveArbTextureFloat,         NO_PROCS,
					"GL_ARB_texture_non_power_of_two", haveArbTextureNonPowerOfTwo, NO_PROCS,
					"GL_ARB_texture_rectangle",        haveArbTextureRectangle,     NO_PROCS,
					"GL_ARB_vertex_buffer_object",     haveArbVertexBufferObject,   PROCS(arbVertexBufferObjectProcs),
					"GL_ARB_vertex_shader",            haveArbVertexShader,         PROCS(arbVertexShaderProcs),
					"GL_ATI_texture_float",            haveAtiTextureFloat,         NO_PROCS,
					"GL_EXT_abgr",                     haveExtAbgr,                 NO_PROCS,
					"GL_EXT_bgra",                     haveExtBgra,                 NO_PROCS,
					"GL_EXT_blend_func_separate",      haveExtBlendFuncSeparate,    PROCS(extBlendFuncSeparateProcs),
					"GL_EXT_blend_minmax",             haveExtBlendMinmax,          PROCS(extBlendMinmaxProcs),
					"GL_EXT_blend_subtract",           haveExtBlendSubtract,        NO_PROCS,
					"GL_EXT_framebuffer_object",       haveExtFramebufferObject,    PROCS(extFramebufferObjectProcs),
					"GL_EXT_texture3d",                haveExtTexture3d,            PROCS(extTexture3dProcs),
					"GL_EXT_texture_edge_clamp",       haveExtTextureEdgeClamp,     NO_PROCS,
					"GL_NV_float_buffer",              haveNvFloatBuffer,           NO_PROCS,
					"GL_NV_register_combiners",        haveNvRegisterCombiners,     PROCS(nvRegisterCombinersProcs),
					"GL_NV_texture_shader",            haveNvTextureShader,         NO_PROCS,
					"GL_SGIS_generate_mipmap",         haveSgisGenerateMipmap,      NO_PROCS,
					"GL_SGIS_texture_edge_clamp",      haveSgisTextureEdgeClamp,    NO_PROCS
				};
			}

			// function binding initialization
			void InitExt()
			{
				std::cout << "loading OpenGL extensions" << std::endl;
				log::Indenter indenter;

				// build supported extension set
				std::string extString(reinterpret_cast<const char *>(glGetString(GL_EXTENSIONS)));
				std::unordered_set<std::string> supportedExts;
				util::Deserialize(extString, std::inserter(supportedExts, supportedExts.end()));

				// calculate extension alignment width
				unsigned width = 0;
				for (const auto &ext : exts)
					width = std::max<decltype(width)>(ext.name.size(), width);

				// set flags for trailing alignment
				boost::io::ios_all_saver iosFormatSaver(std::cout);
				std::cout.setf(std::ios_base::left, std::ios_base::adjustfield);
				std::cout.fill(' ');

				// check relevant extensions
				for (Exts::iterator ext(exts.begin()); ext != exts.end(); ++ext)
				{
					std::cout << std::setw(width) << ext->name << " = ";
					bool broken = false;
					if (ext->have = supportedExts.find(ext->name) != supportedExts.end())
					{
						// check function pointers
						for (Ext::Procs::const_iterator proc(ext->procs.begin()); proc != ext->procs.end(); ++proc)
						{
							if (!(*proc->address = GetProcAddress(proc->name)))
							{
								if (!broken)
								{
									std::cout << "broken" << std::endl;
									broken = true;
									ext->have = false;
								}
								if (*CVAR(logVerbose))
								{
									log::Indenter indenter;
									std::cout << "missing function " << proc->name << std::endl;
								}
							}
						}
					}
					else
					{
						// reset function pointers
						for (Ext::Procs::const_iterator proc(ext->procs.begin()); proc != ext->procs.end(); ++proc)
							*proc->address = nullptr;
					}
					if (!broken) std::cout << (ext->have ? "good" : "missing") << std::endl;
				}
			}
		}
	}
}
