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
#include <iostream> // cout
#include <memory> // unique_ptr

#include <boost/optional.hpp>

#include "../../cache/proxy/Resource.hpp"
#include "../../cfg/vars.hpp"
#include "../../cfg/vars.hpp"
#include "../../err/Exception.hpp"
#include "../../err/report.hpp" // ReportWarning, std::exception
#include "../../math/Color.hpp" // {{,Black,White}Rgba,LuminanceCoefficientRgb}Color
#include "ext.hpp" // ARB_{{fragment,vertex}_shader,shader_objects}, EXT_framebuffer_object
#include "Program.hpp"
#include "ProgramSaver.hpp"
#include "RenderTargetPool.hpp"
#include "Resources.hpp"
#include "resources/ShaderMaterialResources.hpp"
#include "resources/ShaderOutlineResources.hpp"
#include "resources/ShadowResources.hpp"
#include "Texture.hpp"

namespace page
{
	namespace vid
	{
		namespace opengl
		{
			const unsigned blurRenderTargetSize = 128;

			namespace
			{
				// program initialization
				struct ProgramIniter
				{
					explicit ProgramIniter(const std::string &name) :
						name(name) {}

					virtual bool Check() const = 0;
					virtual Program *Make() const = 0;

					std::string name;
				};
				struct Color2alphaProgramIniter : ProgramIniter
				{
					Color2alphaProgramIniter() :
						ProgramIniter("color-to-alpha") {}

					bool Check() const
					{
						return haveArbFragmentShader;
					}
					Program *Make() const
					{
						const std::unique_ptr<Program> program(new Program(
							*cache::Resource<res::opengl::Shader>("shader/glsl/color2alpha.fs")));
						// initialize color weights
						ProgramSaver programSaver;
						Bind(*program);
						glUniform3fvARB(program->GetUniform("weight").location, 1,
							&*(math::LuminanceCoefficientRgbColor<GLfloat>()).begin());
						if (glGetError())
							THROW((err::Exception<err::VidModuleTag, err::OpenglPlatformTag>("failed to initialize uniform")))
						return program.release();
					}
				};
				struct Color2grayProgramIniter : ProgramIniter
				{
					Color2grayProgramIniter() :
						ProgramIniter("color-to-gray") {}

					bool Check() const
					{
						return haveArbFragmentShader;
					}
					Program *Make() const
					{
						const std::unique_ptr<Program> program(new Program(
							*cache::Resource<res::opengl::Shader>("shader/glsl/color2gray.fs")));
						program->GetUniform("grayness");
						// initialize color weights
						ProgramSaver programSaver;
						Bind(*program);
						glUniform3fvARB(program->GetUniform("weight").location, 1,
							&*(math::LuminanceCoefficientRgbColor<GLfloat>()).begin());
						if (glGetError())
							THROW((err::Exception<err::VidModuleTag, err::OpenglPlatformTag>("failed to initialize uniform")))
						return program.release();
					}
				};
				struct ConvolutionFilter3hProgramIniter : ProgramIniter
				{
					ConvolutionFilter3hProgramIniter() :
						ProgramIniter("horizontal 3x convolution-filter") {}

					bool Check() const
					{
						return
							haveArbFragmentShader &&
							haveArbVertexShader;
					}
					Program *Make() const
					{
						const std::unique_ptr<Program> program(new Program(
							*cache::Resource<res::opengl::Shader>("shader/glsl/filter/convolution-3.fs"),
							*cache::Resource<res::opengl::Shader>("shader/glsl/filter/separable-3h.vs")));
						program->GetUniform("kernel");
						program->GetUniform("texSize");
						return program.release();
					}
				};
				struct ConvolutionFilter3vProgramIniter : ProgramIniter
				{
					ConvolutionFilter3vProgramIniter() :
						ProgramIniter("vertical 3x convolution-filter") {}

					bool Check() const
					{
						return
							haveArbFragmentShader &&
							haveArbVertexShader;
					}
					Program *Make() const
					{
						const std::unique_ptr<Program> program(new Program(
							*cache::Resource<res::opengl::Shader>("shader/glsl/filter/convolution-3.fs"),
							*cache::Resource<res::opengl::Shader>("shader/glsl/filter/separable-3v.vs")));
						program->GetUniform("kernel");
						program->GetUniform("texSize");
						return program.release();
					}
				};
				struct ConvolutionFilter5hProgramIniter : ProgramIniter
				{
					ConvolutionFilter5hProgramIniter() :
						ProgramIniter("horizontal 5x convolution-filter") {}

					bool Check() const
					{
						return
							haveArbFragmentShader &&
							haveArbVertexShader;
					}
					Program *Make() const
					{
						const std::unique_ptr<Program> program(new Program(
							*cache::Resource<res::opengl::Shader>("shader/glsl/filter/convolution-5.fs"),
							*cache::Resource<res::opengl::Shader>("shader/glsl/filter/separable-5h.vs")));
						program->GetUniform("kernel");
						program->GetUniform("texSize");
						return program.release();
					}
				};
				struct ConvolutionFilter5vProgramIniter : ProgramIniter
				{
					ConvolutionFilter5vProgramIniter() :
						ProgramIniter("vertical 5x convolution-filter") {}

					bool Check() const
					{
						return
							haveArbFragmentShader &&
							haveArbVertexShader;
					}
					Program *Make() const
					{
						const std::unique_ptr<Program> program(new Program(
							*cache::Resource<res::opengl::Shader>("shader/glsl/filter/convolution-5.fs"),
							*cache::Resource<res::opengl::Shader>("shader/glsl/filter/separable-5v.vs")));
						program->GetUniform("kernel");
						program->GetUniform("texSize");
						return program.release();
					}
				};
				struct ExpConvolutionFilter3hProgramIniter : ProgramIniter
				{
					ExpConvolutionFilter3hProgramIniter() :
						ProgramIniter("horizontal 3x exponential convolution-filter") {}

					bool Check() const
					{
						return
							haveArbFragmentShader &&
							haveArbVertexShader;
					}
					Program *Make() const
					{
						const std::unique_ptr<Program> program(new Program(
							*cache::Resource<res::opengl::Shader>("shader/glsl/filter/exp-convolution-3.fs"),
							*cache::Resource<res::opengl::Shader>("shader/glsl/filter/separable-3h.vs")));
						program->GetUniform("kernel");
						program->GetUniform("texSize");
						return program.release();
					}
				};
				struct ExpConvolutionFilter3vProgramIniter : ProgramIniter
				{
					ExpConvolutionFilter3vProgramIniter() :
						ProgramIniter("vertical 3x exponential convolution-filter") {}

					bool Check() const
					{
						return
							haveArbFragmentShader &&
							haveArbVertexShader;
					}
					Program *Make() const
					{
						const std::unique_ptr<Program> program(new Program(
							*cache::Resource<res::opengl::Shader>("shader/glsl/filter/exp-convolution-3.fs"),
							*cache::Resource<res::opengl::Shader>("shader/glsl/filter/separable-3v.vs")));
						program->GetUniform("kernel");
						program->GetUniform("texSize");
						return program.release();
					}
				};
				struct ExpConvolutionFilter5hProgramIniter : ProgramIniter
				{
					ExpConvolutionFilter5hProgramIniter() :
						ProgramIniter("horizontal 5x exponential convolution-filter") {}

					bool Check() const
					{
						return
							haveArbFragmentShader &&
							haveArbVertexShader;
					}
					Program *Make() const
					{
						const std::unique_ptr<Program> program(new Program(
							*cache::Resource<res::opengl::Shader>("shader/glsl/filter/exp-convolution-5.fs"),
							*cache::Resource<res::opengl::Shader>("shader/glsl/filter/separable-5h.vs")));
						program->GetUniform("kernel");
						program->GetUniform("texSize");
						return program.release();
					}
				};
				struct ExpConvolutionFilter5vProgramIniter : ProgramIniter
				{
					ExpConvolutionFilter5vProgramIniter() :
						ProgramIniter("vertical 5x exponential convolution-filter") {}

					bool Check() const
					{
						return
							haveArbFragmentShader &&
							haveArbVertexShader;
					}
					Program *Make() const
					{
						const std::unique_ptr<Program> program(new Program(
							*cache::Resource<res::opengl::Shader>("shader/glsl/filter/exp-convolution-5.fs"),
							*cache::Resource<res::opengl::Shader>("shader/glsl/filter/separable-5v.vs")));
						program->GetUniform("kernel");
						program->GetUniform("texSize");
						return program.release();
					}
				};
				struct ExpMeanFilter3hProgramIniter : ProgramIniter
				{
					ExpMeanFilter3hProgramIniter() :
						ProgramIniter("horizontal 3x exponential mean-filter") {}

					bool Check() const
					{
						return
							haveArbFragmentShader &&
							haveArbVertexShader;
					}
					Program *Make() const
					{
						const std::unique_ptr<Program> program(new Program(
							*cache::Resource<res::opengl::Shader>("shader/glsl/filter/exp-mean-3.fs"),
							*cache::Resource<res::opengl::Shader>("shader/glsl/filter/separable-3h.vs")));
						program->GetUniform("texSize");
						return program.release();
					}
				};
				struct ExpMeanFilter3vProgramIniter : ProgramIniter
				{
					ExpMeanFilter3vProgramIniter() :
						ProgramIniter("vertical 3x exponential mean-filter") {}

					bool Check() const
					{
						return
							haveArbFragmentShader &&
							haveArbVertexShader;
					}
					Program *Make() const
					{
						const std::unique_ptr<Program> program(new Program(
							*cache::Resource<res::opengl::Shader>("shader/glsl/filter/exp-mean-3.fs"),
							*cache::Resource<res::opengl::Shader>("shader/glsl/filter/separable-3v.vs")));
						program->GetUniform("texSize");
						return program.release();
					}
				};
				struct ExpMeanFilter5hProgramIniter : ProgramIniter
				{
					ExpMeanFilter5hProgramIniter() :
						ProgramIniter("horizontal 5x exponential mean-filter") {}

					bool Check() const
					{
						return
							haveArbFragmentShader &&
							haveArbVertexShader;
					}
					Program *Make() const
					{
						const std::unique_ptr<Program> program(new Program(
							*cache::Resource<res::opengl::Shader>("shader/glsl/filter/exp-mean-5.fs"),
							*cache::Resource<res::opengl::Shader>("shader/glsl/filter/separable-5h.vs")));
						program->GetUniform("texSize");
						return program.release();
					}
				};
				struct ExpMeanFilter5vProgramIniter : ProgramIniter
				{
					ExpMeanFilter5vProgramIniter() :
						ProgramIniter("vertical 5x exponential mean-filter") {}

					bool Check() const
					{
						return
							haveArbFragmentShader &&
							haveArbVertexShader;
					}
					Program *Make() const
					{
						const std::unique_ptr<Program> program(new Program(
							*cache::Resource<res::opengl::Shader>("shader/glsl/filter/exp-mean-5.fs"),
							*cache::Resource<res::opengl::Shader>("shader/glsl/filter/separable-5v.vs")));
						program->GetUniform("texSize");
						return program.release();
					}
				};
				struct MeanFilter3hProgramIniter : ProgramIniter
				{
					MeanFilter3hProgramIniter() :
						ProgramIniter("horizontal 3x mean-filter") {}

					bool Check() const
					{
						return
							haveArbFragmentShader &&
							haveArbVertexShader;
					}
					Program *Make() const
					{
						const std::unique_ptr<Program> program(new Program(
							*cache::Resource<res::opengl::Shader>("shader/glsl/filter/mean-3.fs"),
							*cache::Resource<res::opengl::Shader>("shader/glsl/filter/separable-3h.vs")));
						program->GetUniform("texSize");
						return program.release();
					}
				};
				struct MeanFilter3vProgramIniter : ProgramIniter
				{
					MeanFilter3vProgramIniter() :
						ProgramIniter("vertical 3x mean-filter") {}

					bool Check() const
					{
						return
							haveArbFragmentShader &&
							haveArbVertexShader;
					}
					Program *Make() const
					{
						const std::unique_ptr<Program> program(new Program(
							*cache::Resource<res::opengl::Shader>("shader/glsl/filter/mean-3.fs"),
							*cache::Resource<res::opengl::Shader>("shader/glsl/filter/separable-3v.vs")));
						program->GetUniform("texSize");
						return program.release();
					}
				};
				struct MeanFilter5hProgramIniter : ProgramIniter
				{
					MeanFilter5hProgramIniter() :
						ProgramIniter("horizontal 5x mean-filter") {}

					bool Check() const
					{
						return
							haveArbFragmentShader &&
							haveArbVertexShader;
					}
					Program *Make() const
					{
						const std::unique_ptr<Program> program(new Program(
							*cache::Resource<res::opengl::Shader>("shader/glsl/filter/mean-5.fs"),
							*cache::Resource<res::opengl::Shader>("shader/glsl/filter/separable-5h.vs")));
						program->GetUniform("texSize");
						return program.release();
					}
				};
				struct MeanFilter5vProgramIniter : ProgramIniter
				{
					MeanFilter5vProgramIniter() :
						ProgramIniter("vertical 5x mean-filter") {}

					bool Check() const
					{
						return
							haveArbFragmentShader &&
							haveArbVertexShader;
					}
					Program *Make() const
					{
						const std::unique_ptr<Program> program(new Program(
							*cache::Resource<res::opengl::Shader>("shader/glsl/filter/mean-5.fs"),
							*cache::Resource<res::opengl::Shader>("shader/glsl/filter/separable-5v.vs")));
						program->GetUniform("texSize");
						return program.release();
					}
				};
				struct MedianFilter3ProgramIniter : ProgramIniter
				{
					MedianFilter3ProgramIniter() :
						ProgramIniter("3x3 median-filter") {}

					bool Check() const
					{
						return
							haveArbFragmentShader &&
							haveArbVertexShader;
					}
					Program *Make() const
					{
						const std::unique_ptr<Program> program(new Program(
							*cache::Resource<res::opengl::Shader>("shader/glsl/filter/median-3x3.fs"),
							*cache::Resource<res::opengl::Shader>("shader/glsl/filter/median-3x3.vs")));
						program->GetUniform("texSize");
						return program.release();
					}
				};
				struct NormalProgramIniter : ProgramIniter
				{
					NormalProgramIniter() : ProgramIniter("normal program") {}

					bool Check() const
					{
						return
							haveArbFragmentShader &&
							haveArbVertexShader;
					}
					Program *Make() const
					{
						return new Program(
							*cache::Resource<res::opengl::Shader>("shader/glsl/normal.fs"),
							*cache::Resource<res::opengl::Shader>("shader/glsl/normal.vs"));
					}
				};
				typedef std::array<std::shared_ptr<ProgramIniter>, 20> ProgramIniters;
				inline const ProgramIniters &GetProgramIniters()
				{
					static ProgramIniters initers =
					{
						// NOTE: must match order of enum
						std::shared_ptr<ProgramIniter>(new Color2alphaProgramIniter),
						std::shared_ptr<ProgramIniter>(new Color2grayProgramIniter),
						std::shared_ptr<ProgramIniter>(new ConvolutionFilter3hProgramIniter),
						std::shared_ptr<ProgramIniter>(new ConvolutionFilter3vProgramIniter),
						std::shared_ptr<ProgramIniter>(new ConvolutionFilter5hProgramIniter),
						std::shared_ptr<ProgramIniter>(new ConvolutionFilter5vProgramIniter),
						std::shared_ptr<ProgramIniter>(new ExpConvolutionFilter3hProgramIniter),
						std::shared_ptr<ProgramIniter>(new ExpConvolutionFilter3vProgramIniter),
						std::shared_ptr<ProgramIniter>(new ExpConvolutionFilter5hProgramIniter),
						std::shared_ptr<ProgramIniter>(new ExpConvolutionFilter5vProgramIniter),
						std::shared_ptr<ProgramIniter>(new ExpMeanFilter3hProgramIniter),
						std::shared_ptr<ProgramIniter>(new ExpMeanFilter3vProgramIniter),
						std::shared_ptr<ProgramIniter>(new ExpMeanFilter5hProgramIniter),
						std::shared_ptr<ProgramIniter>(new ExpMeanFilter5vProgramIniter),
						std::shared_ptr<ProgramIniter>(new MeanFilter3hProgramIniter),
						std::shared_ptr<ProgramIniter>(new MeanFilter3vProgramIniter),
						std::shared_ptr<ProgramIniter>(new MeanFilter5hProgramIniter),
						std::shared_ptr<ProgramIniter>(new MeanFilter5vProgramIniter),
						std::shared_ptr<ProgramIniter>(new MedianFilter3ProgramIniter),
						std::shared_ptr<ProgramIniter>(new NormalProgramIniter)
					};
					return initers;
				}

				// texture initialization
				struct TextureIniter
				{
					explicit TextureIniter(const std::string &name) :
						name(name) {}

					virtual bool Check() const = 0;
					virtual Texture *Make() const = 0;

					std::string name;
				};
				struct BlackTextureIniter : TextureIniter
				{
					BlackTextureIniter() : TextureIniter("black") {}

					bool Check() const
					{
						return true;
					}
					Texture *Make() const
					{
						return new Texture(math::BlackRgbaColor());
					}
				};
				struct BlueTextureIniter : TextureIniter
				{
					BlueTextureIniter() : TextureIniter("blue") {}

					bool Check() const
					{
						return true;
					}
					Texture *Make() const
					{
						return new Texture(math::RgbaColor<>(0, 0, 1));
					}
				};
				struct TransparentTextureIniter : TextureIniter
				{
					TransparentTextureIniter() : TextureIniter("transparent") {}

					bool Check() const
					{
						return true;
					}
					Texture *Make() const
					{
						return new Texture(math::RgbaColor<>(0, 0));
					}
				};
				struct WhiteTextureIniter : TextureIniter
				{
					WhiteTextureIniter() : TextureIniter("white") {}

					bool Check() const
					{
						return true;
					}
					Texture *Make() const
					{
						return new Texture(math::WhiteRgbaColor());
					}
				};
				typedef std::array<std::shared_ptr<TextureIniter>, 4> TextureIniters;
				inline const TextureIniters &GetTextureIniters()
				{
					static TextureIniters initers =
					{
						// NOTE: must match order of enum
						std::shared_ptr<TextureIniter>(new BlackTextureIniter),
						std::shared_ptr<TextureIniter>(new BlueTextureIniter),
						std::shared_ptr<TextureIniter>(new TransparentTextureIniter),
						std::shared_ptr<TextureIniter>(new WhiteTextureIniter)
					};
					return initers;
				}

				// render-target pool initialization
				struct RenderTargetPoolIniter
				{
					explicit RenderTargetPoolIniter(const std::string &name) :
						name(name) {}

					virtual bool Check() const = 0;
					virtual RenderTargetPool *Make(const math::Vector<2, unsigned> &size) const = 0;

					std::string name;
				};
				struct RgbBlurRenderTargetPoolIniter : RenderTargetPoolIniter
				{
					RgbBlurRenderTargetPoolIniter() :
						RenderTargetPoolIniter("RGB-blur") {}

					bool Check() const
					{
						return true;
					}
					RenderTargetPool *Make(const math::Vector<2, unsigned> &size) const
					{
						return new RenderTargetPool(
							Ceil(blurRenderTargetSize * math::Vector<2>(Aspect(size), 1)),
							GL_RGB, 1, static_cast<FramebufferFlags>(
								depthFramebufferFlag | filterFramebufferFlag));
					}
				};
				struct RgbaBlurRenderTargetPoolIniter : RenderTargetPoolIniter
				{
					RgbaBlurRenderTargetPoolIniter() :
						RenderTargetPoolIniter("RGBA-blur") {}

					bool Check() const
					{
						return true;
					}
					RenderTargetPool *Make(const math::Vector<2, unsigned> &size) const
					{
						return new RenderTargetPool(
							Ceil(blurRenderTargetSize * math::Vector<2>(Aspect(size), 1)),
							GL_RGBA, 1, static_cast<FramebufferFlags>(
								depthFramebufferFlag | filterFramebufferFlag));
					}
				};
				struct RgbCompositeRenderTargetPoolIniter : RenderTargetPoolIniter
				{
					RgbCompositeRenderTargetPoolIniter() :
						RenderTargetPoolIniter("RGB-compositing") {}

					bool Check() const
					{
						return *CVAR(opengl)::renderComposite;
					}
					RenderTargetPool *Make(const math::Vector<2, unsigned> &size) const
					{
						return new RenderTargetPool(
							size >> *CVAR(opengl)::renderCompositeDown,
							GL_RGB, 1, depthFramebufferFlag);
					}
				};
				struct RgbaCompositeRenderTargetPoolIniter : RenderTargetPoolIniter
				{
					RgbaCompositeRenderTargetPoolIniter() :
						RenderTargetPoolIniter("RGBA-compositing") {}

					bool Check() const
					{
						return *CVAR(opengl)::renderComposite;
					}
					RenderTargetPool *Make(const math::Vector<2, unsigned> &size) const
					{
						return new RenderTargetPool(
							size >> *CVAR(opengl)::renderCompositeDown,
							GL_RGBA, 1, depthFramebufferFlag);
					}
				};
				typedef std::array<std::shared_ptr<RenderTargetPoolIniter>, 4> RenderTargetPoolIniters;
				inline const RenderTargetPoolIniters &GetRenderTargetPoolIniters()
				{
					static RenderTargetPoolIniters initers =
					{
						// NOTE: must match order of enum
						std::shared_ptr<RenderTargetPoolIniter>(new RgbBlurRenderTargetPoolIniter),
						std::shared_ptr<RenderTargetPoolIniter>(new RgbaBlurRenderTargetPoolIniter),
						std::shared_ptr<RenderTargetPoolIniter>(new RgbCompositeRenderTargetPoolIniter),
						std::shared_ptr<RenderTargetPoolIniter>(new RgbaCompositeRenderTargetPoolIniter)
					};
					return initers;
				}
			}

			// construct
			Resources::Resources(const math::Vector<2, unsigned> &sizeForResize)
			{
				// initialize programs
				if (haveArbShaderObjects)
				{
					assert(programs.size() == GetProgramIniters().size());
					Programs::iterator program(programs.begin());
					for (ProgramIniters::const_iterator iter(GetProgramIniters().begin()); iter != GetProgramIniters().end(); ++iter)
					{
						const ProgramIniter &initer(**iter);
						if (initer.Check())
						{
							boost::optional<log::Indenter> indenter;
							if (*CVAR(logVerbose))
							{
								std::cout << "creating " << initer.name << " program" << std::endl;
								indenter = boost::in_place();
							}
							try
							{
								program->reset(initer.Make());
							}
							catch (const std::exception &e)
							{
								err::ReportWarning(e);
							}
							// HACK: must increment here to avoid GCC bug #35182
							program++;
						}
					}
				}
				// initialize textures
				assert(textures.size() == GetTextureIniters().size());
				Textures::iterator texture(textures.begin());
				for (TextureIniters::const_iterator iter(GetTextureIniters().begin()); iter != GetTextureIniters().end(); ++iter)
				{
					const TextureIniter &initer(**iter);
					if (initer.Check())
					{
						boost::optional<log::Indenter> indenter;
						if (*CVAR(logVerbose))
						{
							std::cout << "creating " << initer.name << " texture" << std::endl;
							indenter = boost::in_place();
						}
						try
						{
							texture->reset(initer.Make());
						}
						catch (const std::exception &e)
						{
							err::ReportWarning(e);
						}
						// HACK: must increment here to avoid GCC bug #35182
						++texture;
					}
				}
				// initialize sized resources
				Resize(sizeForResize);
				// initialize shader-render resources
				if (ShaderMaterial::Check())
				{
					std::cout << "initializing material resources" << std::endl;
					log::Indenter indenter;
					try
					{
						shaderMaterial.reset(new ShaderMaterialResources(*this));
					}
					catch (const std::exception &e)
					{
						err::ReportWarning(e);
					}
				}
				// initialize shadow resources
				if (Shadow::Check())
				{
					std::cout << "initializing shadow resources" << std::endl;
					log::Indenter indenter;
					try
					{
						shadow.reset(new ShadowResources);
					}
					catch (const std::exception &e)
					{
						err::ReportWarning(e);
					}
				}
				// check for required resources
				// FIXME: consider falling back on lower quality/correctness
				if (!HasTexture(blackTexture))
					THROW((err::Exception<err::VidModuleTag, err::OpenglPlatformTag>("missing black texture")))
				if (!HasTexture(whiteTexture))
					THROW((err::Exception<err::VidModuleTag, err::OpenglPlatformTag>("missing white texture")))
			}

			// modifiers
			void Resources::Resize(const math::Vector<2, unsigned> &size)
			{
				// reset pointers before reinitializing
				for (RenderTargetPools::iterator renderTargetPool(renderTargetPools.begin()); renderTargetPool != renderTargetPools.end(); ++renderTargetPool)
					renderTargetPool->reset();
				shaderOutline.reset();
				// initialize render-target pools
				if (haveExtFramebufferObject)
				{
					assert(renderTargetPools.size() == GetRenderTargetPoolIniters().size());
					RenderTargetPools::iterator renderTargetPool(renderTargetPools.begin());
					for (RenderTargetPoolIniters::const_iterator iter(GetRenderTargetPoolIniters().begin()); iter != GetRenderTargetPoolIniters().end(); ++iter)
					{
						const RenderTargetPoolIniter &initer(**iter);
						if (initer.Check())
						{
							boost::optional<log::Indenter> indenter;
							if (*CVAR(logVerbose))
							{
								std::cout << "creating " << initer.name << " render-target pool" << std::endl;
								indenter = boost::in_place();
							}
							try
							{
								renderTargetPool->reset(initer.Make(size));
							}
							catch (const std::exception &e)
							{
								err::ReportWarning(e);
							}
							// HACK: must increment here to avoid GCC bug #35182
							renderTargetPool++;
						}
					}
				}
				// initialize shader-outline resources
				if (ShaderOutline::Check())
				{
					std::cout << "initializing outline resources" << std::endl;
					log::Indenter indenter;
					try
					{
						shaderOutline.reset(new ShaderOutlineResources(size));
					}
					catch (const std::exception &e)
					{
						err::ReportWarning(e);
					}
				}
			}

			// general-purpose programs
			bool Resources::HasProgram(Program i) const
			{
				// FIXME: C++0x with strong enums won't need this assertion
				// and index conversion will have to be explicit (N2347)
				assert(i >= 0 && i < programs.size());
				return programs[i] != nullptr;
			}
			const Program &Resources::GetProgram(Program i) const
			{
				// FIXME: C++0x with strong enums won't need this assertion
				// and index conversion will have to be explicit (N2347)
				assert(i >= 0 && i < programs.size());
				return *programs[i];
			}

			// general-purpose render-target pools
			bool Resources::HasRenderTargetPool(RenderTargetPool i) const
			{
				// FIXME: C++0x with strong enums won't need this assertion
				// and index conversion will have to be explicit (N2347)
				assert(i >= 0 && i < renderTargetPools.size());
				return renderTargetPools[i] != nullptr;
			}
			const RenderTargetPool &Resources::GetRenderTargetPool(RenderTargetPool i) const
			{
				// FIXME: C++0x with strong enums won't need this assertion
				// and index conversion will have to be explicit (N2347)
				assert(i >= 0 && i < renderTargetPools.size());
				return *renderTargetPools[i];
			}

			// general-purpose textures
			bool Resources::HasTexture(Texture i) const
			{
				// FIXME: C++0x with strong enums won't need this assertion
				// and index conversion will have to be explicit (N2347)
				assert(i >= 0 && i < textures.size());
				return textures[i] != nullptr;
			}
			const Texture &Resources::GetTexture(Texture i) const
			{
				// FIXME: C++0x with strong enums won't need this assertion
				// and index conversion will have to be explicit (N2347)
				assert(i >= 0 && i < textures.size());
				return *textures[i];
			}

			// shader-outline resources
			bool Resources::HasShaderOutline() const
			{
				return shaderOutline != nullptr;
			}
			const Resources::ShaderOutline &Resources::GetShaderOutline() const
			{
				return *shaderOutline;
			}

			// shader-render resources
			bool Resources::HasShaderMaterial() const
			{
				return shaderMaterial != nullptr;
			}
			const Resources::ShaderMaterial &Resources::GetShaderMaterial() const
			{
				return *shaderMaterial;
			}

			// shadow resources
			bool Resources::HasShadow() const
			{
				return shadow != nullptr;
			}
			const Resources::Shadow &Resources::GetShadow() const
			{
				return *shadow;
			}
		}
	}
}
