/**
 * @copyright
 *
 * Copyright (c) 2006-2014 David Osborn
 *
 * Permission is granted to use and redistribute this software in source and
 * binary form, with or without modification, subject to the following
 * conditions:
 *
 * 1. Redistributions in source form must retain the above copyright notice,
 *    this list of conditions, and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions, and the following disclaimer in the same place
 *    and form as other copyright, license, and disclaimer information.
 *
 * 3. Redistributions in binary form must also include an acknowledgement in the
 *    same place and form as other acknowledgements (such as the credits),
 *    similar in substance to the following:
 *
 *       Portions of this software are based on the work of David Osborn.
 *
 * This software is provided "as is", without any express or implied warranty.
 * In no event will the authors be liable for any damages arising out of the use
 * of this software.
 */

#ifndef    page_local_vid_opengl_ext_hpp
#   define page_local_vid_opengl_ext_hpp

#	include <string>
#	include <GL/gl.h>

#	define GLEXT_PURE
#	include <GL/glext.h>

	// HACK: EXT_texture_edge_clamp is a de-facto standard, but it doesn't exist
	// in the OpenGL extension registry and it may not be included in GL/glext.h
#	ifndef GL_EXT_texture_edge_clamp
#		define GL_EXT_texture_edge_clamp 1
#		define GL_CLAMP_TO_EDGE_EXT 0x812f
#	endif

namespace page
{
	namespace vid
	{
		namespace opengl
		{
			extern bool
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
			extern PFNGLCLAMPCOLORARBPROC glClampColorARB;
			// ARB_depth_texture
			// ARB_draw_buffers
			extern PFNGLDRAWBUFFERSARBPROC glDrawBuffersARB;
			// ARB_fragment_shader
			// ARB_half_float_pixel
			// ARB_multitexture
			extern PFNGLACTIVETEXTUREARBPROC glActiveTextureARB;
			extern PFNGLCLIENTACTIVETEXTUREARBPROC glClientActiveTextureARB;
			extern PFNGLMULTITEXCOORD1DARBPROC glMultiTexCoord1dARB;
			extern PFNGLMULTITEXCOORD1DVARBPROC glMultiTexCoord1dvARB;
			extern PFNGLMULTITEXCOORD1FARBPROC glMultiTexCoord1fARB;
			extern PFNGLMULTITEXCOORD1FVARBPROC glMultiTexCoord1fvARB;
			extern PFNGLMULTITEXCOORD1IARBPROC glMultiTexCoord1iARB;
			extern PFNGLMULTITEXCOORD1IVARBPROC glMultiTexCoord1ivARB;
			extern PFNGLMULTITEXCOORD1SARBPROC glMultiTexCoord1sARB;
			extern PFNGLMULTITEXCOORD1SVARBPROC glMultiTexCoord1svARB;
			extern PFNGLMULTITEXCOORD2DARBPROC glMultiTexCoord2dARB;
			extern PFNGLMULTITEXCOORD2DVARBPROC glMultiTexCoord2dvARB;
			extern PFNGLMULTITEXCOORD2FARBPROC glMultiTexCoord2fARB;
			extern PFNGLMULTITEXCOORD2FVARBPROC glMultiTexCoord2fvARB;
			extern PFNGLMULTITEXCOORD2IARBPROC glMultiTexCoord2iARB;
			extern PFNGLMULTITEXCOORD2IVARBPROC glMultiTexCoord2ivARB;
			extern PFNGLMULTITEXCOORD2SARBPROC glMultiTexCoord2sARB;
			extern PFNGLMULTITEXCOORD2SVARBPROC glMultiTexCoord2svARB;
			extern PFNGLMULTITEXCOORD3DARBPROC glMultiTexCoord3dARB;
			extern PFNGLMULTITEXCOORD3DVARBPROC glMultiTexCoord3dvARB;
			extern PFNGLMULTITEXCOORD3FARBPROC glMultiTexCoord3fARB;
			extern PFNGLMULTITEXCOORD3FVARBPROC glMultiTexCoord3fvARB;
			extern PFNGLMULTITEXCOORD3IARBPROC glMultiTexCoord3iARB;
			extern PFNGLMULTITEXCOORD3IVARBPROC glMultiTexCoord3ivARB;
			extern PFNGLMULTITEXCOORD3SARBPROC glMultiTexCoord3sARB;
			extern PFNGLMULTITEXCOORD3SVARBPROC glMultiTexCoord3svARB;
			extern PFNGLMULTITEXCOORD4DARBPROC glMultiTexCoord4dARB;
			extern PFNGLMULTITEXCOORD4DVARBPROC glMultiTexCoord4dvARB;
			extern PFNGLMULTITEXCOORD4FARBPROC glMultiTexCoord4fARB;
			extern PFNGLMULTITEXCOORD4FVARBPROC glMultiTexCoord4fvARB;
			extern PFNGLMULTITEXCOORD4IARBPROC glMultiTexCoord4iARB;
			extern PFNGLMULTITEXCOORD4IVARBPROC glMultiTexCoord4ivARB;
			extern PFNGLMULTITEXCOORD4SARBPROC glMultiTexCoord4sARB;
			extern PFNGLMULTITEXCOORD4SVARBPROC glMultiTexCoord4svARB;
			// ARB_point_sprite
			// ARB_point_parameters
			extern PFNGLPOINTPARAMETERFARBPROC glPointParameterfARB;
			extern PFNGLPOINTPARAMETERFVARBPROC glPointParameterfvARB;
			// ARB_shader_objects
			extern PFNGLDELETEOBJECTARBPROC glDeleteObjectARB;
			extern PFNGLGETHANDLEARBPROC glGetHandleARB;
			extern PFNGLDETACHOBJECTARBPROC glDetachObjectARB;
			extern PFNGLCREATESHADEROBJECTARBPROC glCreateShaderObjectARB;
			extern PFNGLSHADERSOURCEARBPROC glShaderSourceARB;
			extern PFNGLCOMPILESHADERARBPROC glCompileShaderARB;
			extern PFNGLCREATEPROGRAMOBJECTARBPROC glCreateProgramObjectARB;
			extern PFNGLATTACHOBJECTARBPROC glAttachObjectARB;
			extern PFNGLLINKPROGRAMARBPROC glLinkProgramARB;
			extern PFNGLUSEPROGRAMOBJECTARBPROC glUseProgramObjectARB;
			extern PFNGLVALIDATEPROGRAMARBPROC glValidateProgramARB;
			extern PFNGLUNIFORM1FARBPROC glUniform1fARB;
			extern PFNGLUNIFORM2FARBPROC glUniform2fARB;
			extern PFNGLUNIFORM3FARBPROC glUniform3fARB;
			extern PFNGLUNIFORM4FARBPROC glUniform4fARB;
			extern PFNGLUNIFORM1IARBPROC glUniform1iARB;
			extern PFNGLUNIFORM2IARBPROC glUniform2iARB;
			extern PFNGLUNIFORM3IARBPROC glUniform3iARB;
			extern PFNGLUNIFORM4IARBPROC glUniform4iARB;
			extern PFNGLUNIFORM1FVARBPROC glUniform1fvARB;
			extern PFNGLUNIFORM2FVARBPROC glUniform2fvARB;
			extern PFNGLUNIFORM3FVARBPROC glUniform3fvARB;
			extern PFNGLUNIFORM4FVARBPROC glUniform4fvARB;
			extern PFNGLUNIFORM1IVARBPROC glUniform1ivARB;
			extern PFNGLUNIFORM2IVARBPROC glUniform2ivARB;
			extern PFNGLUNIFORM3IVARBPROC glUniform3ivARB;
			extern PFNGLUNIFORM4IVARBPROC glUniform4ivARB;
			extern PFNGLUNIFORMMATRIX2FVARBPROC glUniformMatrix2fvARB;
			extern PFNGLUNIFORMMATRIX3FVARBPROC glUniformMatrix3fvARB;
			extern PFNGLUNIFORMMATRIX4FVARBPROC glUniformMatrix4fvARB;
			extern PFNGLGETOBJECTPARAMETERFVARBPROC glGetObjectParameterfvARB;
			extern PFNGLGETOBJECTPARAMETERIVARBPROC glGetObjectParameterivARB;
			extern PFNGLGETINFOLOGARBPROC glGetInfoLogARB;
			extern PFNGLGETATTACHEDOBJECTSARBPROC glGetAttachedObjectsARB;
			extern PFNGLGETUNIFORMLOCATIONARBPROC glGetUniformLocationARB;
			extern PFNGLGETACTIVEUNIFORMARBPROC glGetActiveUniformARB;
			extern PFNGLGETUNIFORMFVARBPROC glGetUniformfvARB;
			extern PFNGLGETUNIFORMIVARBPROC glGetUniformivARB;
			extern PFNGLGETSHADERSOURCEARBPROC glGetShaderSourceARB;
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
			extern PFNGLBINDBUFFERARBPROC glBindBufferARB;
			extern PFNGLDELETEBUFFERSARBPROC glDeleteBuffersARB;
			extern PFNGLGENBUFFERSARBPROC glGenBuffersARB;
			extern PFNGLISBUFFERARBPROC glIsBufferARB;
			extern PFNGLBUFFERDATAARBPROC glBufferDataARB;
			extern PFNGLBUFFERSUBDATAARBPROC glBufferSubDataARB;
			extern PFNGLGETBUFFERSUBDATAARBPROC glGetBufferSubDataARB;
			extern PFNGLMAPBUFFERARBPROC glMapBufferARB;
			extern PFNGLUNMAPBUFFERARBPROC glUnmapBufferARB;
			extern PFNGLGETBUFFERPARAMETERIVARBPROC glGetBufferParameterivARB;
			extern PFNGLGETBUFFERPOINTERVARBPROC glGetBufferPointervARB;
			// ARB_vertex_shader
			extern PFNGLVERTEXATTRIB1FARBPROC glVertexAttrib1fARB;
			extern PFNGLVERTEXATTRIB1SARBPROC glVertexAttrib1sARB;
			extern PFNGLVERTEXATTRIB1DARBPROC glVertexAttrib1dARB;
			extern PFNGLVERTEXATTRIB2FARBPROC glVertexAttrib2fARB;
			extern PFNGLVERTEXATTRIB2SARBPROC glVertexAttrib2sARB;
			extern PFNGLVERTEXATTRIB2DARBPROC glVertexAttrib2dARB;
			extern PFNGLVERTEXATTRIB3FARBPROC glVertexAttrib3fARB;
			extern PFNGLVERTEXATTRIB3SARBPROC glVertexAttrib3sARB;
			extern PFNGLVERTEXATTRIB3DARBPROC glVertexAttrib3dARB;
			extern PFNGLVERTEXATTRIB4FARBPROC glVertexAttrib4fARB;
			extern PFNGLVERTEXATTRIB4SARBPROC glVertexAttrib4sARB;
			extern PFNGLVERTEXATTRIB4DARBPROC glVertexAttrib4dARB;
			extern PFNGLVERTEXATTRIB4NUBARBPROC glVertexAttrib4NubARB;
			extern PFNGLVERTEXATTRIB1FVARBPROC glVertexAttrib1fvARB;
			extern PFNGLVERTEXATTRIB1SVARBPROC glVertexAttrib1svARB;
			extern PFNGLVERTEXATTRIB1DVARBPROC glVertexAttrib1dvARB;
			extern PFNGLVERTEXATTRIB2FVARBPROC glVertexAttrib2fvARB;
			extern PFNGLVERTEXATTRIB2SVARBPROC glVertexAttrib2svARB;
			extern PFNGLVERTEXATTRIB2DVARBPROC glVertexAttrib2dvARB;
			extern PFNGLVERTEXATTRIB3FVARBPROC glVertexAttrib3fvARB;
			extern PFNGLVERTEXATTRIB3SVARBPROC glVertexAttrib3svARB;
			extern PFNGLVERTEXATTRIB3DVARBPROC glVertexAttrib3dvARB;
			extern PFNGLVERTEXATTRIB4FVARBPROC glVertexAttrib4fvARB;
			extern PFNGLVERTEXATTRIB4SVARBPROC glVertexAttrib4svARB;
			extern PFNGLVERTEXATTRIB4DVARBPROC glVertexAttrib4dvARB;
			extern PFNGLVERTEXATTRIB4IVARBPROC glVertexAttrib4ivARB;
			extern PFNGLVERTEXATTRIB4BVARBPROC glVertexAttrib4bvARB;
			extern PFNGLVERTEXATTRIB4UBVARBPROC glVertexAttrib4ubvARB;
			extern PFNGLVERTEXATTRIB4USVARBPROC glVertexAttrib4usvARB;
			extern PFNGLVERTEXATTRIB4UIVARBPROC glVertexAttrib4uivARB;
			extern PFNGLVERTEXATTRIB4NBVARBPROC glVertexAttrib4NbvARB;
			extern PFNGLVERTEXATTRIB4NSVARBPROC glVertexAttrib4NsvARB;
			extern PFNGLVERTEXATTRIB4NIVARBPROC glVertexAttrib4NivARB;
			extern PFNGLVERTEXATTRIB4NUBVARBPROC glVertexAttrib4NubvARB;
			extern PFNGLVERTEXATTRIB4NUSVARBPROC glVertexAttrib4NusvARB;
			extern PFNGLVERTEXATTRIB4NUIVARBPROC glVertexAttrib4NuivARB;
			extern PFNGLVERTEXATTRIBPOINTERARBPROC glVertexAttribPointerARB;
			extern PFNGLENABLEVERTEXATTRIBARRAYARBPROC glEnableVertexAttribArrayARB;
			extern PFNGLDISABLEVERTEXATTRIBARRAYARBPROC glDisableVertexAttribArrayARB;
			extern PFNGLBINDATTRIBLOCATIONARBPROC glBindAttribLocationARB;
			extern PFNGLGETACTIVEATTRIBARBPROC glGetActiveAttribARB;
			extern PFNGLGETATTRIBLOCATIONARBPROC glGetAttribLocationARB;
			extern PFNGLGETVERTEXATTRIBDVARBPROC glGetVertexAttribdvARB;
			extern PFNGLGETVERTEXATTRIBFVARBPROC glGetVertexAttribfvARB;
			extern PFNGLGETVERTEXATTRIBIVARBPROC glGetVertexAttribivARB;
			extern PFNGLGETVERTEXATTRIBPOINTERVARBPROC glGetVertexAttribPointervARB;
			// ATI_texture_float
			// EXT_abgr
			// EXT_bgra
			// EXT_blend_func_separate
			extern PFNGLBLENDFUNCSEPARATEEXTPROC glBlendFuncSeparateEXT;
			// EXT_blend_minmax
			extern PFNGLBLENDEQUATIONEXTPROC glBlendEquationEXT;
			// EXT_blend_subtract
			// EXT_framebuffer_object
			extern PFNGLISRENDERBUFFEREXTPROC glIsRenderbufferEXT;
			extern PFNGLBINDRENDERBUFFEREXTPROC glBindRenderbufferEXT;
			extern PFNGLDELETERENDERBUFFERSEXTPROC glDeleteRenderbuffersEXT;
			extern PFNGLGENRENDERBUFFERSEXTPROC glGenRenderbuffersEXT;
			extern PFNGLRENDERBUFFERSTORAGEEXTPROC glRenderbufferStorageEXT;
			extern PFNGLGETRENDERBUFFERPARAMETERIVEXTPROC glGetRenderbufferParameterivEXT;
			extern PFNGLISFRAMEBUFFEREXTPROC glIsFramebufferEXT;
			extern PFNGLBINDFRAMEBUFFEREXTPROC glBindFramebufferEXT;
			extern PFNGLDELETEFRAMEBUFFERSEXTPROC glDeleteFramebuffersEXT;
			extern PFNGLGENFRAMEBUFFERSEXTPROC glGenFramebuffersEXT;
			extern PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC glCheckFramebufferStatusEXT;
			extern PFNGLFRAMEBUFFERTEXTURE1DEXTPROC glFramebufferTexture1DEXT;
			extern PFNGLFRAMEBUFFERTEXTURE2DEXTPROC glFramebufferTexture2DEXT;
			extern PFNGLFRAMEBUFFERTEXTURE3DEXTPROC glFramebufferTexture3DEXT;
			extern PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC glFramebufferRenderbufferEXT;
			extern PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVEXTPROC glGetFramebufferAttachmentParameterivEXT;
			extern PFNGLGENERATEMIPMAPEXTPROC glGenerateMipmapEXT;
			// EXT_texture3D
			extern PFNGLTEXIMAGE3DEXTPROC glTexImage3DEXT;
			// EXT_texture_edge_clamp
			// NV_float_buffer
			// NV_register_combiners
			extern PFNGLCOMBINERPARAMETERFVNVPROC glCombinerParameterfvNV;
			extern PFNGLCOMBINERPARAMETERIVNVPROC glCombinerParameterivNV;
			extern PFNGLCOMBINERPARAMETERFNVPROC glCombinerParameterfNV;
			extern PFNGLCOMBINERPARAMETERINVPROC glCombinerParameteriNV;
			extern PFNGLCOMBINERINPUTNVPROC glCombinerInputNV;
			extern PFNGLCOMBINEROUTPUTNVPROC glCombinerOutputNV;
			extern PFNGLFINALCOMBINERINPUTNVPROC glFinalCombinerInputNV;
			extern PFNGLGETCOMBINERINPUTPARAMETERFVNVPROC glGetCombinerInputParameterfvNV;
			extern PFNGLGETCOMBINERINPUTPARAMETERIVNVPROC glGetCombinerInputParameterivNV;
			extern PFNGLGETCOMBINEROUTPUTPARAMETERFVNVPROC glGetCombinerOutputParameterfvNV;
			extern PFNGLGETCOMBINEROUTPUTPARAMETERIVNVPROC glGetCombinerOutputParameterivNV;
			extern PFNGLGETFINALCOMBINERINPUTPARAMETERFVNVPROC glGetFinalCombinerInputParameterfvNV;
			extern PFNGLGETFINALCOMBINERINPUTPARAMETERIVNVPROC glGetFinalCombinerInputParameterivNV;
			// NV_texture_shader
			// SGIS_generate_mipmap
			// SGIS_texture_edge_clamp

			// function binding initialization
			// must be called before using extensions
			void InitExt();

			// function address resolution
			// must be implemented in platform layer
			typedef void (*ProcAddress)();
			ProcAddress GetProcAddress(const std::string &);
		}
	}
}

#	undef GLEXT_PURE
#	include <GL/glext.h>
#endif
