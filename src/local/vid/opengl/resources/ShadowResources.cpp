#include <algorithm> // rotate
#include <array>
#include <cassert>
#include <iostream> // cout
#include <memory> // {shared,unique}_ptr
#include <string>

#include <boost/optional.hpp>

#include "../../../cache/proxy/ResourceProxy.hpp"
#include "../../../cfg/vars.hpp"
#include "../../../cfg/vars.hpp"
#include "../../../err/Exception.hpp"
#include "../../../err/report.hpp" // ReportWarning, std::exception
#include "../../../log/Indenter.hpp"
#include "../../../math/Vector.hpp"
#include "../ext.hpp" // ARB_{{fragment,vertex}_shader,shader_objects,texture_float}, ATI_texture_float, EXT_framebuffer_object
#include "../Program.hpp"
#include "../RenderTargetPool.hpp"
#include "ShadowResources.hpp"

namespace page
{
	namespace vid
	{
		namespace opengl
		{
			const unsigned renderTargetSize = 2048;

			namespace
			{
				// type initialization
				struct TypeIniter
				{
					TypeIniter(ShadowResources::Type type, const std::string &name) :
						type(type), name(name) {}

					virtual bool Check() const = 0;
					virtual Program *MakeProgram() const = 0;
					virtual RenderTargetPool *MakeRenderTargetPool(unsigned size) const = 0;

					ShadowResources::Type type;
					std::string name;
				};
				struct ExponentialTypeIniter : TypeIniter
				{
					ExponentialTypeIniter() :
						TypeIniter(ShadowResources::exponentialType, "exponential") {}

					bool Check() const
					{
						return
							haveArbTextureFloat ||
							haveAtiTextureFloat;
					}
					Program *MakeProgram() const
					{
						return new Program(
							*cache::ResourceProxy<res::opengl::Shader>("shader/glsl/shadow/shadow.fs"),
							*cache::ResourceProxy<res::opengl::Shader>("shader/glsl/shadow/shadow.vs"),
							*cache::ResourceProxy<res::opengl::Shader>("shader/glsl/shadow/shadow-exponential.fs"),
							*cache::ResourceProxy<res::opengl::Shader>("shader/glsl/shadow/shadow-exponential.vs"));
					}
					RenderTargetPool *MakeRenderTargetPool(unsigned size) const
					{
						// find compatible color format
						// FIXME: can we use a single-component format?
						GLenum colorFormat;
						if      (haveArbTextureFloat) colorFormat = GL_RGBA16F_ARB;
						else if (haveAtiTextureFloat) colorFormat = GL_RGBA_FLOAT16_ATI;
						else assert(!"no compatible color format");
						// select filtering mode
						RenderTargetPool::Flags flags = depthFramebufferFlag;
						if (*CVAR(opengl)::renderShadowFilter)
							flags = static_cast<RenderTargetPool::Flags>(
								flags | filterFramebufferFlag);
						// create best compatible render-target pool
						for (;;)
						{
							try
							{
								return new RenderTargetPool(size, colorFormat, 1, flags);
							}
							catch (const std::exception &e)
							{
								if (flags & filterFramebufferFlag)
								{
									err::ReportWarning(e);
									if (*CVAR(logVerbose))
										std::cout << "falling back to unfiltered mode" << std::endl;
									flags = static_cast<RenderTargetPool::Flags>(
										flags ^ filterFramebufferFlag);
								}
								else throw;
							}
						}
					}
				};
				struct PackedTypeIniter : TypeIniter
				{
					PackedTypeIniter() :
						TypeIniter(ShadowResources::packedType, "packed") {}

					bool Check() const
					{
						return true;
					}
					Program *MakeProgram() const
					{
						const std::unique_ptr<Program> program(new Program(
							*cache::ResourceProxy<res::opengl::Shader>("shader/glsl/shadow/shadow.fs"),
							*cache::ResourceProxy<res::opengl::Shader>("shader/glsl/shadow/shadow.vs"),
							*cache::ResourceProxy<res::opengl::Shader>("shader/glsl/shadow/shadow-packed.fs"),
							*cache::ResourceProxy<res::opengl::Shader>("shader/glsl/shadow/shadow-packed.vs")));
						program->GetUniform("invFar");
						return program.release();
					}
					RenderTargetPool *MakeRenderTargetPool(unsigned size) const
					{
						return new RenderTargetPool(size, GL_RGBA8, 1, depthFramebufferFlag);
					}
				};
				struct VarianceTypeIniter : TypeIniter
				{
					VarianceTypeIniter() :
						TypeIniter(ShadowResources::varianceType, "variance") {}

					bool Check() const
					{
						return
							haveArbTextureFloat ||
							haveAtiTextureFloat;
					}
					Program *MakeProgram() const
					{
						return new Program(
							*cache::ResourceProxy<res::opengl::Shader>("shader/glsl/shadow/shadow.fs"),
							*cache::ResourceProxy<res::opengl::Shader>("shader/glsl/shadow/shadow.vs"),
							*cache::ResourceProxy<res::opengl::Shader>("shader/glsl/shadow/shadow-variance.fs"),
							*cache::ResourceProxy<res::opengl::Shader>("shader/glsl/shadow/shadow-variance.vs"));
					}
					RenderTargetPool *MakeRenderTargetPool(unsigned size) const
					{
						// find compatible color format
						GLenum colorFormat;
						if      (haveArbTextureFloat) colorFormat = GL_RGBA16F_ARB;
						else if (haveAtiTextureFloat) colorFormat = GL_RGBA_FLOAT16_ATI;
						else assert(!"no compatible color format");
						// select filtering mode
						RenderTargetPool::Flags flags = depthFramebufferFlag;
						if (*CVAR(opengl)::renderShadowFilter)
							flags = static_cast<RenderTargetPool::Flags>(
								flags | filterFramebufferFlag);
						// create best compatible render-target pool
						for (;;)
						{
							try
							{
								return new RenderTargetPool(size, colorFormat, 1, flags);
							}
							catch (const std::exception &e)
							{
								if (flags & filterFramebufferFlag)
								{
									err::ReportWarning(e);
									if (*CVAR(logVerbose))
										std::cout << "falling back to unfiltered mode" << std::endl;
									flags = static_cast<RenderTargetPool::Flags>(
										flags ^ filterFramebufferFlag);
								}
								else throw;
							}
						}
					}
				};
				typedef std::array<std::shared_ptr<TypeIniter>, 3> TypeIniters;
				inline const TypeIniters &GetTypeIniters()
				{
					static TypeIniters initers =
					{
						// NOTE: should be ordered by preference
						std::shared_ptr<TypeIniter>(new ExponentialTypeIniter),
						std::shared_ptr<TypeIniter>(new VarianceTypeIniter),
						std::shared_ptr<TypeIniter>(new PackedTypeIniter)
					};
					return initers;
				}
			}

			// construct
			ShadowResources::ShadowResources()
			{
				// order selected type first
				TypeIniters initers(GetTypeIniters());
				for (TypeIniters::iterator iter(initers.begin()); iter != initers.end(); ++iter)
				{
					const TypeIniter &initer(**iter);
					// FIXME: C++0x with strong enums won't need this
					// static_cast (N2347)
					if (initer.type == static_cast<ShadowResources::Type>(*CVAR(opengl)::renderShadowType))
					{
						std::rotate(initers.begin(), iter, iter + 1);
						break;
					}
				}
				// initialize first compatible type
				for (TypeIniters::const_iterator iter(initers.begin()); iter != initers.end(); ++iter)
				{
					const TypeIniter &initer(**iter);
					if (!initer.Check()) continue;
					std::cout << "using " << initer.name << " shadow mapping" << std::endl;
					log::Indenter indenter;
					try
					{
						// create program
						boost::optional<log::Indenter> indenter;
						if (*CVAR(logVerbose))
						{
							std::cout << "creating program" << std::endl;
							indenter = boost::in_place();
						}
						try
						{
							program.reset(initer.MakeProgram());
						}
						catch (const std::exception &e)
						{
							err::ReportWarning(e);
							throw;
						}
						// create render-target pool
						if (*CVAR(logVerbose))
						{
							indenter = boost::none;
							std::cout << "creating render-target pool" << std::endl;
							indenter = boost::in_place();
						}
						try
						{
							renderTargetPool.reset(initer.MakeRenderTargetPool(
								renderTargetSize >> *CVAR(opengl)::renderShadowDown));
						}
						catch (const std::exception &e)
						{
							err::ReportWarning(e);
							throw;
						}
					}
					catch (...)
					{
						program.reset();
						renderTargetPool.reset();
						continue;
					}
					assert(program);
					assert(renderTargetPool);
					type = initer.type;
					return;
				}
				THROW((err::Exception<err::VidModuleTag, err::OpenglPlatformTag>("no compatible shadow type")))
			}

			// access
			ShadowResources::Type ShadowResources::GetType() const
			{
				return type;
			}
			const Program &ShadowResources::GetProgram() const
			{
				return *program;
			}
			const RenderTargetPool &ShadowResources::GetRenderTargetPool() const
			{
				return *renderTargetPool;
			}

			// compatibility
			bool ShadowResources::Check()
			{
				// NOTE: this should be enough to ensure compatibility with at
				// least one shadow type
				return
					*CVAR(opengl)::renderShadow &&
					haveArbFragmentShader &&
					haveArbShaderObjects &&
					haveArbVertexShader &&
					haveExtFramebufferObject;
			}
		}
	}
}
