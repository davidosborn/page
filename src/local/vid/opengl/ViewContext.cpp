#include <algorithm> // min
#include <cassert>
#include <cmath> // cos, sin

#include <GL/gl.h>

#include "../../cache/proxy/AabbProxy.hpp"
#include "../../cache/proxy/opengl/DrawableProxy.hpp"
#include "../../cache/proxy/opengl/TextureProxy.hpp"
#include "../../cfg/vars.hpp"
#include "../../cfg/vars.hpp"
#include "../../math/Color.hpp" // Rgb{,a}Color
#include "../../math/float.hpp" // DegToRad
#include "../../math/interp.hpp" // HermiteConvolutionKernel
#include "../../math/OrthoFrustum.hpp"
#include "../../math/Vector.hpp"
#include "../../math/ViewFrustum.hpp"
#include "../../phys/Form.hpp" // Form::{Get{Matrix,Parts},IsPosed,Part::{Get{Form,Material,Matrix},IsDeformed}}
#include "../../phys/Light.hpp" // Light::Get{Ambient,Cutoff,Diffuse,{Max,Min}Range,Normal,Position,Specular}
#include "../../phys/mixin/Collidable.hpp" // Collidable::Get{Position,Radius}
#include "../../res/type/Track.hpp"
#include "../Driver.hpp" // Driver::GetViewport
#include "activeTexture.hpp" // {,Can}AllocActiveTexture, GetActiveTextureIndex
#include "ActiveTextureSaver.hpp"
#include "blend.hpp" // ColorBlendFunc
#include "Drawable.hpp" // Drawable::Draw
#include "DrawContext.hpp" // DrawContext->vid::DrawContext, DrawContext::{{Begin,End}VolatileMatrix,Fill,Get{Frame,Resources}}
#include "Driver.hpp" // Driver->vid::Driver
#include "ext.hpp" // ARB_{multitexture,shader_objects,vertex_buffer_object}, EXT_blend_func_separate
#include "Framebuffer.hpp" // Framebuffer::GetPow2Size
#include "MatrixGuard.hpp"
#include "Program.hpp" // Bind, Program::GetUniform
#include "ProgramSaver.hpp"
#include "RenderTarget.hpp" // RenderTarget::framebuffer
#include "RenderTargetPool.hpp" // Bind, RenderTargetPool::{Get{,Size},Pooled}
#include "RenderTargetSaver.hpp"
#include "Resources.hpp" // Resources::{{Get,Has}{Program,RenderTargetPool,Shader{Outline,Render},Shadow,Texture},convolutionFilter[35][hv]Program,{rgbaComposite,rgbBlur}RenderTargetPool,whiteTexture}
#include "resources/ShaderMaterialResources.hpp" // ShaderMaterialResources::{GetProgram,MaterialMask}
#include "resources/ShaderOutlineResources.hpp" // ShaderOutlineResources::Get{Program,RenderTargetPool}
#include "resources/ShadowResources.hpp" // ShadowResources::{Get{Program,RenderTargetPool,Type},Type}
#include "Texture.hpp" // Bind
#include "ViewContext.hpp"

namespace page
{
	namespace vid
	{
		namespace opengl
		{
			const float
				shadowNear = -40,
				shadowFar  = 40,
				shadowSize = 40;

			namespace
			{
				// texture binding
				void Bind(const res::Material::Pass::Texture &texture,
					MatrixGuard &matrixGuard, TextureFormat format = defaultTextureFormat)
				{
					Bind(*cache::opengl::TextureProxy(texture.image, format));
					if (Any(texture.offset) || Any(texture.scale != 1))
					{
						glMatrixMode(GL_TEXTURE);
						matrixGuard.Push();
						glTranslatef(texture.offset.x, texture.offset.y, 0);
						glScalef(texture.scale.x, texture.scale.y, 0);
					}
				}
			}

			// construct/destroy
			ViewContext::ViewContext(Base &base, const math::ViewFrustum<> &frustum) :
				vid::ViewContext(base, frustum)
			{
				// setup frame
				// NOTE: moving frame into projection matrix to leave modelview
				// matrix free for 3D positioning
				glMatrixMode(GL_PROJECTION);
				matrixGuard.Push();
				math::Aabb<2> frame(base.GetFrame());
				glTranslatef(frame.min.x, frame.min.y, 0);
				math::Vec2 frameSize(Size(frame));
				glScalef(frameSize.x, frameSize.y, 1);
				glOrtho(-3, 1, 3, -1, -1, 1);
				// setup view frustum
				glMultMatrixf(&*math::Matrix<4, 4, GLfloat>(GetProjMatrix(frustum)).begin());
				glMatrixMode(GL_MODELVIEW);
				matrixGuard.Push();
				glLoadMatrixf(&*math::Matrix<4, 4, GLfloat>(GetViewMatrix(frustum)).begin());
				// set default state
				glPushAttrib(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				glAlphaFunc(GL_GEQUAL, .5);
				glEnable(GL_ALPHA_TEST);
				glDisable(GL_BLEND);
				glEnable(GL_DEPTH_TEST);
				glClear(GL_DEPTH_BUFFER_BIT);
				// mark matrix volatility
				base.BeginVolatileMatrix();
			}
			ViewContext::~ViewContext()
			{
				GetBase().EndVolatileMatrix();
			}

			// base context access
			ViewContext::Base &ViewContext::GetBase()
			{
				return static_cast<Base &>(vid::ViewContext::GetBase());
			}
			const ViewContext::Base &ViewContext::GetBase() const
			{
				return static_cast<const Base &>(vid::ViewContext::GetBase());
			}

			// scene rendering
			void ViewContext::Draw(const phys::Scene &scene)
			{
				const Resources &res(GetBase().GetResources());
				// retrieve visible forms
				typedef phys::Scene::View<phys::Form>::Type Forms;
				Forms forms(scene.GetVisibleForms(GetFrustum()));
				// write early depth pass
				// FIXME: this has a detrimental effect right now
				/*{
					AttribGuard attribGuard;
					glPushAttrib(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
					glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
					glDisable(GL_BLEND);
					Draw(forms, zcullFixedType);
				}
				glDepthFunc(GL_EQUAL);
				glDepthMask(GL_FALSE);*/
				// retrieve scene lights
				typedef phys::Scene::View<phys::Light>::Type Lights;
				Lights lights(scene.GetInfluentialLights(GetFrustum()));
				// select rendering path
				if (*CVAR(opengl)::renderShader && res.HasShaderMaterial())
				{
					// perform shadow mapping
					boost::optional<ShadowAttachment> shadowAttachment;
					if (*CVAR(opengl)::renderShadow && res.HasShadow())
					{
						math::Vec2u shadowRenderTargetSize(
							res.GetShadow().GetRenderTargetPool().GetSize());
						math::Vec3 shadowTexelSize(
							shadowSize / shadowRenderTargetSize,
							// FIXME: it shouldn't be necessary to round in the
							// direction of the shadow
							(shadowFar - shadowNear) / Max(shadowRenderTargetSize));
						math::Mat3 sunMatrix(LookMatrix(scene.GetSunDirection(), math::NormVector<3>()));
						math::OrthoFrustum<> shadowFrustum(
							shadowNear, shadowFar, shadowSize, shadowSize,
							sunMatrix * Round(Tpos(sunMatrix) * scene.GetFocus(), shadowTexelSize),
							math::Quat<>(sunMatrix));
						shadowAttachment = DrawShadow(shadowFrustum, forms);
					}
					// initialize lighting
					AttribGuard attribGuard;
					glPushAttrib(GL_LIGHTING_BIT);
					glLightModelfv(GL_LIGHT_MODEL_AMBIENT, &*math::RgbaColor<GLfloat>(scene.GetAmbient()).begin());
					glLightfv(GL_LIGHT0, GL_POSITION, &*math::Vector<4, GLfloat>(-scene.GetSunDirection(), 0).begin());
					// draw opaque forms
					Draw(forms, basicShaderType, shadowAttachment);
					// draw transparent forms
					// FIXME: implement
					// draw emissive and specular glow
					// TEST: disabled until we actually need emissive/specular
					/*if (*CVAR(opengl)::renderGlow &&
						res.HasProgram(Resources::convolutionFilter5hProgram) &&
						res.HasProgram(Resources::convolutionFilter5vProgram) &&
						res.HasRenderTargetPool(Resources::rgbBlurRenderTargetPool))
					{
						// draw emissive and specular to first glow buffer
						AttribGuard attribGuard;
						glPushAttrib(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
						RenderTargetSaver renderTargetSaver;
						Bind(glowRes.GetBuffer());
						glClearColor(0, 0, 0, 0);
						glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
						Draw(forms, emissiveSpecularShaderType, shadowAttachment);
						// copy to second glow buffer with horizontal blur
						Bind(glowRes.GetBuffer(), 1);
						ProgramSaver programSaver;
						Bind(blurRes.Get5hProgram());
						glUniform1iARB(
							blurRes.Get5hProgram().GetUniform("texSize").location,
							glowRes.GetBuffer().GetPow2Size().x);
						glDisable(GL_ALPHA_TEST);
						glDisable(GL_DEPTH_TEST);
						GetBase().Fill(glowRes.GetBuffer());
						// add to parent buffer buffer with vertical blur
						bufferSaver.Reset();
						Bind(blurRes.Get5vProgram());
						glUniform1iARB(
							blurRes.Get5vProgram().GetUniform("texSize").location,
							glowRes.GetBuffer().GetPow2Size().y);
						glBlendFunc(GL_ONE, GL_ONE);
						glEnable(GL_BLEND);
						GetBase().Fill(glowRes.GetBuffer(), 1);
					}*/
					// draw outlines
					if (*CVAR(opengl)::renderOutline &&
						res.HasShaderOutline() &&
						res.HasProgram(Resources::normalProgram))
					{
						const Resources::ShaderOutline &outlineRes(res.GetShaderOutline());
						const Program &outlineProgram(outlineRes.GetProgram());
						RenderTargetPool::Pooled outlineRenderTarget(outlineRes.GetRenderTargetPool().Get());
						// render normals to first outline buffer
						AttribGuard attribGuard;
						glPushAttrib(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
						RenderTargetSaver renderTargetSaver;
						Bind(outlineRenderTarget);
						glClearColor(0, 0, 1, 0);
						glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
						Draw(forms, normalShaderType);
						// copy to parent buffer with edge detection
						renderTargetSaver.Reset();
						ProgramSaver programSaver;
						Bind(outlineProgram);
						glUniform2ivARB(
							outlineProgram.GetUniform("texSize").location, 1,
							&*math::Vector<2, GLint>(outlineRenderTarget->framebuffer.GetPow2Size()).begin());
						glDisable(GL_ALPHA_TEST);
						glDisable(GL_DEPTH_TEST);
						glBlendFunc(GL_DST_COLOR, GL_ZERO);
						glEnable(GL_BLEND);
						GetBase().Fill(*outlineRenderTarget);
					}
				}
				else // fixed-function path
				{
					AttribGuard attribGuard;
					glPushAttrib(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_LIGHTING_BIT);
					// initialize lighting
					glLightModelfv(GL_LIGHT_MODEL_AMBIENT, &*math::RgbaColor<GLfloat>(scene.GetAmbient()).begin());
					glLightfv(GL_LIGHT0, GL_POSITION, &*math::Vector<4, GLfloat>(-scene.GetSunDirection(), 0).begin());
					glEnable(GL_COLOR_MATERIAL);
					glEnable(GL_LIGHT0);
					glEnable(GL_LIGHTING);
					// draw opaque forms
					Draw(forms, basicFixedType);
					// draw transparent forms
					glDepthMask(GL_FALSE);
					if (res.HasRenderTargetPool(Resources::rgbaCompositeRenderTargetPool))
					{
						glDisable(GL_ALPHA_TEST);
						glBlendFunc(GL_ONE, GL_ONE);
						glEnable(GL_BLEND);
						// FIXME: implement; composite using alpha buffer
					}
					else
					{
						// FIXME: implement; render opaque if opacity >= 50%
					}
					// draw outlines
					if (*CVAR(opengl)::renderOutline)
					{
						glDisable(GL_LIGHTING);
						glDisable(GL_ALPHA_TEST);
						glBlendFunc(GL_ONE, GL_ONE);
						glEnable(GL_BLEND);
						// FIXME: implement; use backface line primitives
					}
				}
				// draw debug overlays
				if (*CVAR(debugDrawTrack) && scene.HasTrack()) Draw(scene.GetTrack());
				if (*CVAR(debugDrawCollision)) Draw(scene.GetVisibleCollidables(GetFrustum()));
				if (*CVAR(debugDrawBounds)) DrawBounds(forms);
				if (*CVAR(debugDrawSkeleton)) DrawSkeleton(forms);
			}

			// mesh rendering
			void ViewContext::Draw(const phys::Form &form, ShaderType type, const boost::optional<ShadowAttachment> &shadow)
			{
				using std::bind;
				using namespace std::placeholders;
				Draw(form, bind(&ViewContext::PrepShaderMaterial, this, _1, _2, type, shadow),
					*CVAR(opengl)::renderMultipass && type != shadowShaderType);
			}
			void ViewContext::Draw(const phys::Form &form, FixedType type)
			{
				using std::bind;
				using namespace std::placeholders;
				Draw(form, bind(&ViewContext::PrepFixedMaterial, this, _1, _2, type),
					*CVAR(opengl)::renderMultipass && type != zcullFixedType);
			}
			void ViewContext::Draw(const phys::Scene::View<phys::Form>::Type &forms, ShaderType type, const boost::optional<ShadowAttachment> &shadow)
			{
				typedef phys::Scene::View<phys::Form>::Type Forms;
				for (Forms::const_iterator iter(forms.begin()); iter != forms.end(); ++iter)
				{
					const phys::Form &form(**iter);
					Draw(form, type, shadow);
				}
			}
			void ViewContext::Draw(const phys::Scene::View<phys::Form>::Type &forms, FixedType type)
			{
				typedef phys::Scene::View<phys::Form>::Type Forms;
				for (Forms::const_iterator iter(forms.begin()); iter != forms.end(); ++iter)
				{
					const phys::Form &form(**iter);
					Draw(form, type);
				}
			}

			// mesh rendering implementation
			void ViewContext::Draw(const phys::Form &form, const PrepMaterialCallback &prepMaterialCallback, bool multipass)
			{
				// set matrix
				glMatrixMode(GL_MODELVIEW);
				MatrixGuard matrixGuard;
				matrixGuard.Push();
				glMultMatrixf(&*math::Matrix<4, 4, GLfloat>(form.GetMatrix()).begin());
				// draw parts
				for (phys::Form::Parts::const_iterator part(form.GetParts().begin()); part != form.GetParts().end(); ++part)
					Draw(*part, prepMaterialCallback, multipass);
			}
			void ViewContext::Draw(const phys::Form::Part &part, const PrepMaterialCallback &prepMaterialCallback, bool multipass)
			{
				// set matrix
				glMatrixMode(GL_MODELVIEW);
				MatrixGuard matrixGuard;
				matrixGuard.Push();
				glMultMatrixf(&*math::Matrix<4, 4, GLfloat>(part.GetMatrix()).begin());
				// draw part with material
				ProgramSaver programSaver;
				if (part.GetMaterial())
				{
					const res::Material &mat(*part.GetMaterial());
					if (multipass && mat.passes.size() > 1)
					{
						// draw first pass to establish base
						{
							ActiveTextureSaver activeTextureSaver;
							AttribGuard attribGuard;
							MatrixGuard matrixGuard;
							Draw(part, prepMaterialCallback(mat.passes.front(), matrixGuard));
						}
						// blend in remaining passes
						AttribGuard attribGuard;
						glPushAttrib(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
						glDisable(GL_ALPHA_TEST);
						if (haveExtBlendFuncSeparate)
							ColorBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
						else glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
						glEnable(GL_BLEND);
						glDepthFunc(GL_EQUAL);
						glDepthMask(GL_FALSE);
						for (res::Material::Passes::const_iterator pass(mat.passes.begin() + 1); pass != mat.passes.end(); ++pass)
						{
							ActiveTextureSaver activeTextureSaver;
							AttribGuard attribGuard;
							glPushAttrib(GL_COLOR_BUFFER_BIT);
							MatrixGuard matrixGuard;
							Draw(part, prepMaterialCallback(*pass, matrixGuard));
						}
					}
					else
					{
						// use single material pass
						ActiveTextureSaver activeTextureSaver;
						AttribGuard attribGuard;
						MatrixGuard matrixGuard;
						Draw(part, prepMaterialCallback(mat.passes.front(), matrixGuard));
					}
				}
				else
				{
					// use default material
					ActiveTextureSaver activeTextureSaver;
					AttribGuard attribGuard;
					MatrixGuard matrixGuard;
					Draw(part, prepMaterialCallback(res::Material::Pass(), matrixGuard));
				}
			}
			void ViewContext::Draw(const phys::Form::Part &part, const VertexFormat &vertexFormat)
			{
				cache::opengl::DrawableProxy(part)->Draw(vertexFormat);
			}

			// shader material setup
			VertexFormat ViewContext::PrepShaderMaterial(const res::Material::Pass &pass, MatrixGuard &matrixGuard, ShaderType type, const boost::optional<ShadowAttachment> &shadow)
			{
				assert(haveArbMultitexture);
				assert(!GetActiveTextureIndex());
				switch (type)
				{
					case basicShaderType: return PrepBasicShaderMaterial(pass, matrixGuard, shadow);
					case emissiveSpecularShaderType: return PrepEmissiveSpecularShaderMaterial(pass, matrixGuard, shadow);
					case normalShaderType: return PrepNormalShaderMaterial(pass, matrixGuard);
					case shadowShaderType: return PrepShadowShaderMaterial(pass, matrixGuard);
					default: assert(!"invalid shader type");
				}
			}
			VertexFormat ViewContext::PrepBasicShaderMaterial(const res::Material::Pass &pass, MatrixGuard &matrixGuard, const boost::optional<ShadowAttachment> &shadow)
			{
				VertexFormat vertexFormat;
				vertexFormat.normal = true;
				glPushAttrib(GL_CURRENT_BIT);
				// initialize shader
				const Resources &res(GetBase().GetResources());
				const Resources::ShaderMaterial &shaderMaterialResources(res.GetShaderMaterial());
				Resources::ShaderMaterial::MaterialMask mask = Resources::ShaderMaterial::allMaterialMask;
				if (!UseEmissiveSpecularGlow())
					mask = static_cast<Resources::ShaderMaterial::MaterialMask>(
						mask ^ (
							Resources::ShaderMaterial::emissiveMaterialMask |
							Resources::ShaderMaterial::specularMaterialMask));
				if (!shadow)
					mask = static_cast<Resources::ShaderMaterial::MaterialMask>(
						mask ^ Resources::ShaderMaterial::shadowMaterialMask);
				const Program &program(*shaderMaterialResources.GetProgram(pass, mask));
				Bind(program);
				// initialize diffuse texture
				if (const Program::Uniform *uniform = program.FindUniform("diffuseSampler"))
				{
					assert(CanAllocActiveTexture());
					AllocActiveTexture();
					if (pass.diffuse.texture.image)
						Bind(pass.diffuse.texture, matrixGuard);
					else Bind(res.GetTexture(Resources::whiteTexture));
					vertexFormat.uvIndices.push_back(pass.diffuse.texture.uvIndex);
					glUniform1iARB(uniform->location, GetActiveTextureIndex());
				}
				// initialize diffuse color and mask value
				math::RgbaColor<GLfloat> color(pass.diffuse.color);
				color.a *= pass.mask.value;
				glColor4fv(&*color.begin());
				// initialize emissive texture
				// FIXME: implement
				// initialize emissive color
				// FIXME: implement
				// initialize fresnel texture
				// FIXME: implement
				// initialize fresnel color
				// FIXME: implement
				// initialize gloss texture
				// FIXME: implement
				// initialize gloss value
				// FIXME: implement
				// initialize mask texture
				if (const Program::Uniform *uniform = program.FindUniform("maskSampler"))
				{
					assert(CanAllocActiveTexture());
					AllocActiveTexture();
					if (pass.mask.texture.image)
						Bind(pass.mask.texture, matrixGuard, alphaTextureFormat);
					else Bind(res.GetTexture(Resources::whiteTexture));
					vertexFormat.uvIndices.push_back(pass.mask.texture.uvIndex);
					glUniform1iARB(uniform->location, GetActiveTextureIndex());
				}
				// initialize normal texture
				if (const Program::Uniform *uniform = program.FindUniform("normalSampler"))
				{
					// FIXME: implement
					vertexFormat.tangent = true;
				}
				// initialize specular texture
				// FIXME: implement
				// initialize specular color
				// FIXME: implement
				// initialize shadow texture
				if (const Program::Uniform *uniform = program.FindUniform("shadowSampler"))
				{
					assert(CanAllocActiveTexture());
					AllocActiveTexture();
					if (shadow)
					{
						glBindTexture(GL_TEXTURE_2D, GetTexture(shadow->renderTarget));
						if (const Program::Uniform *uniform = program.FindUniform("shadowMatrix"))
							glUniformMatrix4fvARB(uniform->location, 1, GL_FALSE,
								&*math::Matrix<4, 4, GLfloat>(shadow->matrix).begin());
					}
					else Bind(res.GetTexture(Resources::whiteTexture));
					glUniform1iARB(uniform->location, GetActiveTextureIndex());
				}
				return vertexFormat;
			}
			VertexFormat ViewContext::PrepEmissiveSpecularShaderMaterial(const res::Material::Pass &pass, MatrixGuard &matrixGuard, const boost::optional<ShadowAttachment> &shadow)
			{
				VertexFormat vertexFormat;
				vertexFormat.normal = true;
				glPushAttrib(GL_CURRENT_BIT);
				// initialize shader
				const Resources &res(GetBase().GetResources());
				const Resources::ShaderMaterial &shaderMaterialResources(res.GetShaderMaterial());
				Resources::ShaderMaterial::MaterialMask mask =
					static_cast<Resources::ShaderMaterial::MaterialMask>(
						Resources::ShaderMaterial::emissiveMaterialMask |
						Resources::ShaderMaterial::specularMaterialMask);
				if (shadow)
					mask = static_cast<Resources::ShaderMaterial::MaterialMask>(
						mask | Resources::ShaderMaterial::shadowMaterialMask);
				const Program &program(*shaderMaterialResources.GetProgram(pass, mask));
				Bind(program);
				// initialize ambient color
				if (const Program::Uniform *uniform = program.FindUniform("ambientColor"))
					glUniform3fvARB(uniform->location, 1, &*math::RgbColor<GLfloat>().begin());
				// initialize diffuse texture
				if (const Program::Uniform *uniform = program.FindUniform("diffuseSampler"))
				{
					assert(CanAllocActiveTexture());
					AllocActiveTexture();
					if (pass.diffuse.texture.image)
						Bind(pass.diffuse.texture, matrixGuard);
					else Bind(res.GetTexture(Resources::whiteTexture));
					vertexFormat.uvIndices.push_back(pass.diffuse.texture.uvIndex);
					glUniform1iARB(uniform->location, GetActiveTextureIndex());
				}
				// initialize diffuse color and mask value
				glColor4f(0, 0, 0, pass.diffuse.color.a * pass.mask.value);
				// initialize emissive texture
				// FIXME: implement
				// initialize emissive color
				// FIXME: implement
				// initialize mask texture
				if (const Program::Uniform *uniform = program.FindUniform("maskSampler"))
				{
					assert(CanAllocActiveTexture());
					AllocActiveTexture();
					if (pass.mask.texture.image)
						Bind(pass.mask.texture, matrixGuard, alphaTextureFormat);
					else Bind(res.GetTexture(Resources::whiteTexture));
					vertexFormat.uvIndices.push_back(pass.mask.texture.uvIndex);
					glUniform1iARB(uniform->location, GetActiveTextureIndex());
				}
				// initialize specular texture
				// FIXME: implement
				// initialize specular color
				// FIXME: implement
				// initialize shadow texture
				if (const Program::Uniform *uniform = program.FindUniform("shadowSampler"))
				{
					assert(CanAllocActiveTexture());
					AllocActiveTexture();
					if (shadow)
					{
						glBindTexture(GL_TEXTURE_2D, GetTexture(shadow->renderTarget));
						if (const Program::Uniform *uniform = program.FindUniform("shadowMatrix"))
							glUniformMatrix4fvARB(uniform->location, 1, GL_FALSE,
								&*math::Matrix<4, 4, GLfloat>(shadow->matrix).begin());
					}
					else Bind(res.GetTexture(Resources::whiteTexture));
					glUniform1iARB(uniform->location, GetActiveTextureIndex());
				}
				return vertexFormat;
			}
			VertexFormat ViewContext::PrepNormalShaderMaterial(const res::Material::Pass &pass, MatrixGuard &matrixGuard)
			{
				VertexFormat vertexFormat;
				vertexFormat.normal = true;
				glPushAttrib(GL_CURRENT_BIT);
				// initialize shader
				const Resources &res(GetBase().GetResources());
				const Program &program(res.GetProgram(Resources::normalProgram));
				Bind(program);
				// initialize diffuse color and mask value
				glColor4f(1, 1, 1, pass.diffuse.color.a * pass.mask.value);
				// initialize diffuse texture
				if (const Program::Uniform *uniform = program.FindUniform("diffuseSampler"))
				{
					assert(CanAllocActiveTexture());
					AllocActiveTexture();
					if (pass.diffuse.texture.image)
						Bind(pass.diffuse.texture, matrixGuard);
					else Bind(res.GetTexture(Resources::whiteTexture));
					vertexFormat.uvIndices.push_back(pass.diffuse.texture.uvIndex);
					glUniform1iARB(uniform->location, GetActiveTextureIndex());
				}
				// initialize mask texture
				if (const Program::Uniform *uniform = program.FindUniform("maskSampler"))
				{
					assert(CanAllocActiveTexture());
					AllocActiveTexture();
					if (pass.mask.texture.image)
						Bind(pass.mask.texture, matrixGuard, alphaTextureFormat);
					else Bind(res.GetTexture(Resources::whiteTexture));
					vertexFormat.uvIndices.push_back(pass.mask.texture.uvIndex);
					glUniform1iARB(uniform->location, GetActiveTextureIndex());
				}
				return vertexFormat;
			}
			VertexFormat ViewContext::PrepShadowShaderMaterial(const res::Material::Pass &pass, MatrixGuard &matrixGuard)
			{
				VertexFormat vertexFormat;
				glPushAttrib(GL_CURRENT_BIT);
				// initialize shader
				const Resources &res(GetBase().GetResources());
				const Resources::Shadow &shadowRes(res.GetShadow());
				const Program &program(shadowRes.GetProgram());
				Bind(program);
				// initialize diffuse color and mask value
				glColor4f(1, 1, 1, pass.diffuse.color.a * pass.mask.value);
				// initialize diffuse texture
				if (const Program::Uniform *uniform = program.FindUniform("diffuseSampler"))
				{
					assert(CanAllocActiveTexture());
					AllocActiveTexture();
					if (pass.diffuse.texture.image)
						Bind(pass.diffuse.texture, matrixGuard);
					else Bind(res.GetTexture(Resources::whiteTexture));
					vertexFormat.uvIndices.push_back(pass.diffuse.texture.uvIndex);
					glUniform1iARB(uniform->location, GetActiveTextureIndex());
				}
				// initialize mask texture
				if (const Program::Uniform *uniform = program.FindUniform("maskSampler"))
				{
					assert(CanAllocActiveTexture());
					AllocActiveTexture();
					if (pass.mask.texture.image)
						Bind(pass.mask.texture, matrixGuard, alphaTextureFormat);
					else Bind(res.GetTexture(Resources::whiteTexture));
					vertexFormat.uvIndices.push_back(pass.mask.texture.uvIndex);
					glUniform1iARB(uniform->location, GetActiveTextureIndex());
				}
				return vertexFormat;
			}

			// fixed-function material setup
			VertexFormat ViewContext::PrepFixedMaterial(const res::Material::Pass &pass, MatrixGuard &matrixGuard, FixedType type)
			{
				switch (type)
				{
					case basicFixedType: return PrepBasicFixedMaterial(pass, matrixGuard);
					case zcullFixedType: return PrepZcullFixedMaterial(pass, matrixGuard);
					default: assert(!"invalid fixed-function type");
				}
			}
			VertexFormat ViewContext::PrepBasicFixedMaterial(const res::Material::Pass &pass, MatrixGuard &matrixGuard)
			{
				VertexFormat vertexFormat;
				vertexFormat.normal = true;
				glPushAttrib(GL_CURRENT_BIT);
				// initialize diffuse color and mask value
				math::RgbaColor<GLfloat> color(pass.diffuse.color);
				color.a *= pass.mask.value;
				glColor4fv(&*color.begin());
				// initialize textures
				if (haveArbMultitexture)
				{
					assert(!GetActiveTextureIndex());
					glPushAttrib(GL_TEXTURE_BIT);
					// initialize diffuse texture
					if (pass.diffuse.texture.image && CanAllocActiveTexture())
					{
						AllocActiveTexture();
						Bind(pass.diffuse.texture, matrixGuard);
						glEnable(GL_TEXTURE_2D);
						vertexFormat.uvIndices.push_back(pass.diffuse.texture.uvIndex);
					}
					// initialize emissive texture
					// FIXME: implement
					// initialize emissive color
					// FIXME: implement
					// initialize mask texture
					if (pass.mask.texture.image && CanAllocActiveTexture())
					{
						AllocActiveTexture();
						Bind(pass.mask.texture, matrixGuard, alphaTextureFormat);
						glEnable(GL_TEXTURE_2D);
						vertexFormat.uvIndices.push_back(pass.mask.texture.uvIndex);
					}
					// initialize specular texture
					// FIXME: implement
					// initialize specular color
					// FIXME: implement
				}
				else
				{
					glPushAttrib(GL_ENABLE_BIT);
					// initialize diffuse texture
					if (pass.diffuse.texture.image)
					{
						Bind(pass.diffuse.texture, matrixGuard);
						glEnable(GL_TEXTURE_2D);
						vertexFormat.uvIndices.push_back(pass.diffuse.texture.uvIndex);
					}
				}
				return vertexFormat;
			}
			VertexFormat ViewContext::PrepZcullFixedMaterial(const res::Material::Pass &pass, MatrixGuard &matrixGuard)
			{
				VertexFormat vertexFormat;
				// initialize diffuse color and mask value
				math::RgbaColor<GLfloat> color(pass.diffuse.color);
				color.a *= pass.mask.value;
				glColor4fv(&*color.begin());
				// initialize textures
				if (haveArbMultitexture)
				{
					assert(!GetActiveTextureIndex());
					glPushAttrib(GL_ENABLE_BIT);
					// initialize diffuse texture
					if (pass.diffuse.texture.image && CanAllocActiveTexture())
					{
						AllocActiveTexture();
						Bind(pass.diffuse.texture, matrixGuard);
						glEnable(GL_TEXTURE_2D);
						vertexFormat.uvIndices.push_back(pass.diffuse.texture.uvIndex);
					}
					// initialize mask texture
					if (pass.mask.texture.image && CanAllocActiveTexture())
					{
						AllocActiveTexture();
						Bind(pass.mask.texture, matrixGuard, alphaTextureFormat);
						glEnable(GL_TEXTURE_2D);
						vertexFormat.uvIndices.push_back(pass.mask.texture.uvIndex);
					}
				}
				else
				{
					glPushAttrib(GL_ENABLE_BIT);
					// initialize diffuse texture
					if (pass.diffuse.texture.image)
					{
						Bind(pass.diffuse.texture, matrixGuard);
						glEnable(GL_TEXTURE_2D);
						vertexFormat.uvIndices.push_back(pass.diffuse.texture.uvIndex);
					}
				}
				return vertexFormat;
			}

			// shadow rendering
			ViewContext::ShadowAttachment ViewContext::DrawShadow(const math::OrthoFrustum<> &frustum, const phys::Scene::View<phys::Form>::Type &forms)
			{
				const Resources &res(GetBase().GetResources());
				const Resources::Shadow &shadowRes(res.GetShadow());
				// set shadow matrix
				glMatrixMode(GL_PROJECTION);
				MatrixGuard matrixGuard;
				matrixGuard.Push();
				glLoadMatrixf(&*math::Matrix<4, 4, GLfloat>(GetProjMatrix(frustum)).begin());
				glMatrixMode(GL_MODELVIEW);
				matrixGuard.Push();
				glLoadMatrixf(&*math::Matrix<4, 4, GLfloat>(GetViewMatrix(frustum)).begin());
				// render shadow map
				const RenderTargetPool &shadowRenderTargetPool(shadowRes.GetRenderTargetPool());
				RenderTargetPool::Pooled shadowRenderTarget(shadowRenderTargetPool.Get());
				switch (shadowRes.GetType())
				{
					case Resources::Shadow::exponentialType:
					case Resources::Shadow::varianceType:
					{
						AttribGuard attribGuard;
						glPushAttrib(GL_COLOR_BUFFER_BIT);
						RenderTargetSaver renderTargetSaver;
						Bind(shadowRenderTarget);
						glDisable(GL_ALPHA_TEST);
						glDisable(GL_BLEND);
						glClearColor(1, 1, 1, 1);
						glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
						Draw(forms, shadowShaderType);
					}
					break;
					case Resources::Shadow::packedType:
					{
						AttribGuard attribGuard;
						glPushAttrib(GL_COLOR_BUFFER_BIT);
						RenderTargetSaver renderTargetSaver;
						Bind(shadowRenderTarget);
						glDisable(GL_ALPHA_TEST);
						glDisable(GL_BLEND);
						glClearColor(1, 1, 1, 1);
						glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
						Draw(forms, shadowShaderType);
					}
					break;
					default: assert(!"invalid shadow type");
				}
				// blur shadow map
				if (*CVAR(opengl)::renderShadowBlur)
				{
					const Program
						*blurhProgram = 0,
						*blurvProgram = 0;
					switch (shadowRes.GetType())
					{
						case Resources::Shadow::exponentialType:
						if (!res.HasProgram(Resources::expMeanFilter3hProgram) ||
							!res.HasProgram(Resources::expMeanFilter3vProgram))
							goto NoShadow;
						blurhProgram = &res.GetProgram(Resources::expMeanFilter3hProgram);
						blurvProgram = &res.GetProgram(Resources::expMeanFilter3vProgram);
						break;
						case Resources::Shadow::varianceType:
						if (!res.HasProgram(Resources::meanFilter3hProgram) ||
							!res.HasProgram(Resources::meanFilter3vProgram))
							goto NoShadow;
						blurhProgram = &res.GetProgram(Resources::meanFilter3hProgram);
						blurvProgram = &res.GetProgram(Resources::meanFilter3vProgram);
						break;
						default: goto NoShadow;
					}
					assert(blurhProgram && blurvProgram);
					RenderTargetPool::Pooled blurRenderTarget(shadowRenderTargetPool.Get());
					// perform horizontal blur
					AttribGuard attribGuard;
					glPushAttrib(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
					RenderTargetSaver renderTargetSaver;
					Bind(blurRenderTarget);
					ProgramSaver programSaver;
					Bind(*blurhProgram);
					glUniform1iARB(
						blurhProgram->GetUniform("texSize").location,
						shadowRenderTargetPool.GetPow2Size().x);
					glDisable(GL_ALPHA_TEST);
					glDisable(GL_DEPTH_TEST);
					glDisable(GL_BLEND);
					GetBase().Fill(*shadowRenderTarget);
					// perform vertical blur
					Bind(shadowRenderTarget);
					Bind(*blurvProgram);
					glUniform1iARB(
						blurvProgram->GetUniform("texSize").location,
						shadowRenderTargetPool.GetPow2Size().y);
					GetBase().Fill(*blurRenderTarget);
				}
				NoShadow:;
				// return shadow attachment
				ShadowAttachment shadowAttachment =
				{
					shadowRenderTarget,
					GetMatrix(frustum) * GetInvViewMatrix(GetFrustum())
				};
				return shadowAttachment;
			}

			// debug rendering
			void ViewContext::Draw(const res::Track &track)
			{
				// bias height to avoid Z fighting
				glMatrixMode(GL_MODELVIEW);
				MatrixGuard matrixGuard;
				matrixGuard.Push();
				glTranslatef(0, .001, 0);
				// draw overlay
				AttribGuard attribGuard;
				glPushAttrib(GL_COLOR_BUFFER_BIT | GL_CURRENT_BIT | GL_DEPTH_BUFFER_BIT | GL_POLYGON_BIT);
				glDisable(GL_ALPHA_TEST);
				glDepthMask(GL_FALSE);
				if (haveExtBlendFuncSeparate)
					ColorBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				else glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				glEnable(GL_BLEND);
				glColor4f(1, 0, 0, .25);
				glBegin(GL_TRIANGLES);
				for (res::Track::Faces::const_iterator face(track.faces.begin()); face != track.faces.end(); ++face)
					for (res::Track::Face::Vertices::const_iterator vertex(face->vertices.begin()); vertex != face->vertices.end(); ++vertex)
						glVertex3fv(&*math::Vector<3, GLfloat>(*vertex).begin());
				glEnd();
				// draw wireframe
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				glColor4f(0, 0, 0, .25);
				glBegin(GL_TRIANGLES);
				for (res::Track::Faces::const_iterator face(track.faces.begin()); face != track.faces.end(); ++face)
					for (res::Track::Face::Vertices::const_iterator vertex(face->vertices.begin()); vertex != face->vertices.end(); ++vertex)
						glVertex3fv(&*math::Vector<3, GLfloat>(*vertex).begin());
				glEnd();
				// draw boundaries
				glColor3f(1, 1, 1);
				glBegin(GL_LINES);
				for (res::Track::Faces::const_iterator face(track.faces.begin()); face != track.faces.end(); ++face)
					for (unsigned i = 0; i < 3; ++i)
						if (!face->neighbours[i])
						{
							glVertex3fv(&*math::Vector<3, GLfloat>(face->vertices[i]).begin());
							glVertex3fv(&*math::Vector<3, GLfloat>(face->vertices[(i + 1) % 3]).begin());
						}
				glEnd();
			}
			void ViewContext::Draw(const phys::Scene::View<phys::Collidable>::Type &collidables)
			{
				// bias height to avoid Z fighting
				glMatrixMode(GL_MODELVIEW);
				MatrixGuard matrixGuard;
				matrixGuard.Push();
				glTranslatef(0, .001, 0);
				// draw collidable disks
				AttribGuard attribGuard;
				glPushAttrib(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				glDisable(GL_ALPHA_TEST);
				glDepthMask(GL_FALSE);
				if (haveExtBlendFuncSeparate)
					ColorBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				else glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				glEnable(GL_BLEND);
				typedef phys::Scene::View<phys::Collidable>::Type Collidables;
				for (Collidables::const_iterator iter(collidables.begin()); iter != collidables.end(); ++iter)
				{
					const phys::Collidable &collidable(**iter);
					math::Vec3 center(collidable.GetPosition());
					float radius = collidable.GetRadius();
					glBegin(GL_LINE_LOOP);
					for (float i = 0; i < 360; i += 20)
						glVertex3f(
							center.x + std::cos(math::DegToRad(i)) * radius,
							center.y,
							center.z + std::sin(math::DegToRad(i)) * radius);
					glEnd();
				}
			}
			void ViewContext::DrawBounds(const phys::Scene::View<phys::Form>::Type &forms)
			{
				typedef const phys::Scene::View<phys::Form>::Type Forms;
				for (Forms::const_iterator iter(forms.begin()); iter != forms.end(); ++iter)
				{
					const phys::Form &form(**iter);
					DrawBounds(form);
				}
			}
			void ViewContext::DrawBounds(const phys::Form &form)
			{
				if (!form.GetModel()) return;
				math::Aabb<3> aabb(*cache::AabbProxy(form));
				AttribGuard attribGuard;
				glPushAttrib(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				glDisable(GL_ALPHA_TEST);
				glDisable(GL_DEPTH_TEST);
				if (haveExtBlendFuncSeparate)
					ColorBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				else glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				glEnable(GL_BLEND);
				glBegin(GL_LINE_LOOP);
				glVertex3f(aabb.min.x, aabb.min.y, aabb.min.z);
				glVertex3f(aabb.min.x, aabb.min.y, aabb.max.z);
				glVertex3f(aabb.max.x, aabb.min.y, aabb.max.z);
				glVertex3f(aabb.max.x, aabb.min.y, aabb.min.z);
				glEnd();
				glBegin(GL_LINE_LOOP);
				glVertex3f(aabb.min.x, aabb.max.y, aabb.min.z);
				glVertex3f(aabb.min.x, aabb.max.y, aabb.max.z);
				glVertex3f(aabb.max.x, aabb.max.y, aabb.max.z);
				glVertex3f(aabb.max.x, aabb.max.y, aabb.min.z);
				glEnd();
				glBegin(GL_LINES);
				glVertex3f(aabb.min.x, aabb.min.y, aabb.min.z);
				glVertex3f(aabb.min.x, aabb.max.y, aabb.min.z);
				glVertex3f(aabb.min.x, aabb.min.y, aabb.max.z);
				glVertex3f(aabb.min.x, aabb.max.y, aabb.max.z);
				glVertex3f(aabb.max.x, aabb.min.y, aabb.max.z);
				glVertex3f(aabb.max.x, aabb.max.y, aabb.max.z);
				glVertex3f(aabb.max.x, aabb.min.y, aabb.min.z);
				glVertex3f(aabb.max.x, aabb.max.y, aabb.min.z);
				glEnd();
			}
			void ViewContext::DrawSkeleton(const phys::Scene::View<phys::Form>::Type &forms)
			{
				typedef phys::Scene::View<phys::Form>::Type Forms;
				for (Forms::const_iterator iter(forms.begin()); iter != forms.end(); ++iter)
				{
					const phys::Form &form(**iter);
					DrawSkeleton(form);
				}
			}
			void ViewContext::DrawSkeleton(const phys::attrib::Pose &pose)
			{
				glMatrixMode(GL_MODELVIEW);
				MatrixGuard matrixGuard;
				matrixGuard.Push();
				glMultMatrixf(&*math::Matrix<4, 4, GLfloat>(pose.GetMatrix()).begin());
				// draw posed skeleton
				AttribGuard attribGuard;
				glPushAttrib(GL_COLOR_BUFFER_BIT | GL_CURRENT_BIT | GL_DEPTH_BUFFER_BIT);
				glDisable(GL_ALPHA_TEST);
				glDisable(GL_DEPTH_TEST);
				if (haveExtBlendFuncSeparate)
					ColorBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				else glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				glEnable(GL_BLEND);
				glBegin(GL_LINES);
				for (phys::Form::ConstBoneIterator bone(pose.BeginBones()); bone != pose.EndBones(); ++bone)
				{
					math::Vector<3, GLfloat> pos(GetTranslation(bone->GetPoseMatrix()));
					// draw basis
					math::Vector<3, GLfloat> axes[3] =
					{
						bone->GetPoseMatrix() * math::Vec3(.1, 0, 0),
						bone->GetPoseMatrix() * math::Vec3(0, .1, 0),
						bone->GetPoseMatrix() * math::Vec3(0, 0, .1)
					};
					glColor4f(1.f, 0.f, 0.f, .5);
					glVertex3fv(&*pos.begin());
					glVertex3fv(&*axes[0].begin());
					glColor4f(0.f, 1.f, 0.f, .5);
					glVertex3fv(&*pos.begin());
					glVertex3fv(&*axes[1].begin());
					glColor4f(0.f, 0.f, 1.f, .5);
					glVertex3fv(&*pos.begin());
					glVertex3fv(&*axes[2].begin());
					// draw connections
					if (const phys::Form::Bone *parent = bone->GetParent())
					{
						math::Vector<3, GLfloat> end(GetTranslation(parent->GetPoseMatrix()));
						glColor4f(0.f, 0.f, 0.f, .5f);
						glVertex3fv(&*pos.begin());
						glVertex3fv(&*end.begin());
					}
				}
				glEnd();
			}

			// decisions
			bool ViewContext::UseEmissiveSpecularGlow() const
			{
				// FIXME: methinks this function is not good because it is so
				// distant from the actual implementation such that if the
				// implementation changes, this function may be forgotten
				const Resources &res(GetBase().GetResources());
				return
					*CVAR(opengl)::renderGlow &&
					res.HasProgram(Resources::convolutionFilter5hProgram) &&
					res.HasProgram(Resources::convolutionFilter5vProgram) &&
					res.HasRenderTargetPool(Resources::rgbBlurRenderTargetPool);
			}
		}
	}
}
