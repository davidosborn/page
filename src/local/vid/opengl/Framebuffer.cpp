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

#include <algorithm> // transform
#include <cassert>
#include "../../env/Window.hpp" // Window::GetSize
#include "../../err/Exception.hpp"
#include "../../math/pow2.hpp" // Pow2Ceil
#include "AttribGuard.hpp"
#include "DrawContext.hpp" // DrawContext::GetDriver
#include "Driver.hpp" // Driver::GetWindow
#include "ext.hpp" // ARB_texture_non_power_of_two, EXT_framebuffer_object, {EXT,SGIS}_texture_edge_clamp
#include "Framebuffer.hpp"
#include "get.hpp" // GetInteger

namespace page
{
	namespace vid
	{
		namespace opengl
		{
			// construct/destroy
			Framebuffer::Framebuffer(const math::Vector<2, unsigned> &size, GLenum colorFormat, Flags flags) :
				size(size), pow2Size(size), renderbuffer(0),
				colorFormat(colorFormat), flags(flags)
			{
				assert(haveExtFramebufferObject);
				// calculate compatible size
				if (!haveArbTextureNonPowerOfTwo)
					std::transform(pow2Size.begin(), pow2Size.end(), pow2Size.begin(), math::Pow2Ceil);
				// create framebuffer
				glGenFramebuffersEXT(1, &framebuffer);
				glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, framebuffer);
				if (glGetError())
				{
					glDeleteFramebuffersEXT(1, &framebuffer);
					THROW((err::Exception<err::VidModuleTag, err::OpenglPlatformTag>("failed to create framebuffer")))
				}
				// attach renderbuffer
				if (flags & depthFramebufferFlag)
				{
					glGenRenderbuffersEXT(1, &renderbuffer);
					glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, renderbuffer);
					glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, pow2Size.x, pow2Size.y);
					if (glGetError())
					{
						glDeleteRenderbuffersEXT(1, &renderbuffer);
						glDeleteFramebuffersEXT(1, &framebuffer);
						THROW((err::Exception<err::VidModuleTag, err::OpenglPlatformTag>("failed to create renderbuffer")))
					}
					glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);
					glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, renderbuffer);
					if (glGetError())
					{
						glDeleteRenderbuffersEXT(1, &renderbuffer);
						glDeleteFramebuffersEXT(1, &framebuffer);
						THROW((err::Exception<err::VidModuleTag, err::OpenglPlatformTag>("failed to attach renderbuffer")))
					}
				}
				// verify framebuffer
				try
				{
					Verify();
				}
				catch (...)
				{
					glDeleteRenderbuffersEXT(1, &renderbuffer);
					glDeleteFramebuffersEXT(1, &framebuffer);
					throw;
				}
				glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
			}
			Framebuffer::~Framebuffer()
			{
				glDeleteFramebuffersEXT(1, &framebuffer);
				glDeleteTextures(colorTextures.size(), &*colorTextures.begin());
				glDeleteRenderbuffersEXT(1, &renderbuffer);
			}

			// attributes
			const math::Vector<2, unsigned> &Framebuffer::GetSize() const
			{
				return size;
			}
			const math::Vector<2, unsigned> &Framebuffer::GetPow2Size() const
			{
				return pow2Size;
			}

			// handle access
			GLuint Framebuffer::GetHandle() const
			{
				return framebuffer;
			}
			GLuint Framebuffer::GetRenderbuffer() const
			{
				return renderbuffer;
			}
			GLuint Framebuffer::GetTexture(GLenum attachment) const
			{
				assert(attachment >= GL_COLOR_ATTACHMENT0_EXT);
				unsigned i = attachment - GL_COLOR_ATTACHMENT0_EXT;
				assert(i < colorTextures.size());
				return colorTextures[i];
			}

			// attachment modifiers
			GLenum Framebuffer::AttachColorTexture()
			{
				GLuint texture;
				glGenTextures(1, &texture);
				if (glGetError())
					THROW((err::Exception<err::VidModuleTag, err::OpenglPlatformTag>("failed to generate texture")))
				glBindTexture(GL_TEXTURE_2D, texture);
				glTexImage2D(GL_TEXTURE_2D, 0, colorFormat, pow2Size.x, pow2Size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
				// set texture parameters
				if (flags & filterFramebufferFlag)
				{
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				}
				else
				{
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				}
				if (haveExtTextureEdgeClamp || haveSgisTextureEdgeClamp)
				{
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE_SGIS);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE_SGIS);
				}
				else
				{
					// HACK: this will not prevent bottom-right edge
					// filtering on non-power-of-two textures
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
				}
				if (glGetError())
				{
					glDeleteTextures(1, &texture);
					THROW((err::Exception<err::VidModuleTag, err::OpenglPlatformTag>("failed to initialize texture")))
				}
				// attach texture to framebuffer
				glBindTexture(GL_TEXTURE_2D, 0);
				GLuint framebufferBinding = GetInteger(GL_FRAMEBUFFER_BINDING_EXT);
				glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, framebuffer);
				GLenum colorAttachment = GL_COLOR_ATTACHMENT0_EXT + colorTextures.size();
				glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, colorAttachment, GL_TEXTURE_2D, texture, 0);
				if (glGetError())
				{
					glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, framebufferBinding);
					glDeleteTextures(1, &texture);
					THROW((err::Exception<err::VidModuleTag, err::OpenglPlatformTag>("failed to attach texture")))
				}
				try
				{
					Verify();
					colorTextures.push_back(texture);
				}
				catch (...)
				{
					glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, colorAttachment, GL_TEXTURE_2D, texture, 0);
					glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, framebufferBinding);
					glDeleteTextures(1, &texture);
					throw;
				}
				glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, framebufferBinding);
				return colorAttachment;
			}

			// completeness
			void Framebuffer::Verify() const
			{
				assert(GetInteger(GL_FRAMEBUFFER_BINDING_EXT) == framebuffer);
				// NOTE: buffer state partially affects framebuffer completeness
				// (EXT_framebuffer_object issue #55 and #66)
				AttribGuard attribGuard;
				glPushAttrib(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_PIXEL_MODE_BIT);
				if (!colorTextures.empty())
				{
					GLenum colorAttachment = GL_COLOR_ATTACHMENT0_EXT + colorTextures.size() - 1;
					glDrawBuffer(colorAttachment);
					glReadBuffer(colorAttachment);
				}
				else
				{
					glDrawBuffer(GL_NONE);
					glReadBuffer(GL_NONE);
				}
				if (flags & depthFramebufferFlag) glEnable(GL_DEPTH_TEST);
				else glDisable(GL_DEPTH_TEST);
				if (!CheckStatus())
					THROW((err::Exception<err::VidModuleTag, err::OpenglPlatformTag>("framebuffer format unsupported")))
			}
			bool Framebuffer::CheckStatus()
			{
				switch (glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT))
				{
					case GL_FRAMEBUFFER_COMPLETE_EXT: return true;
					case GL_FRAMEBUFFER_UNSUPPORTED_EXT: return false;
					case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT:
					THROW((err::Exception<err::VidModuleTag, err::OpenglPlatformTag>("incomplete framebuffer attachment")))
					case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT:
					THROW((err::Exception<err::VidModuleTag, err::OpenglPlatformTag>("missing framebuffer attachment")))
					case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
					THROW((err::Exception<err::VidModuleTag, err::OpenglPlatformTag>("framebuffer size mismatch")))
					case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
					THROW((err::Exception<err::VidModuleTag, err::OpenglPlatformTag>("framebuffer format mismatch")))
					case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT:
					THROW((err::Exception<err::VidModuleTag, err::OpenglPlatformTag>("framebuffer not drawable")))
					case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT:
					THROW((err::Exception<err::VidModuleTag, err::OpenglPlatformTag>("framebuffer not readable")))
				}
				THROW((err::Exception<err::VidModuleTag, err::OpenglPlatformTag>("framebuffer error")))
			}

			// binding
			void Bind(const Framebuffer &fb)
			{
				glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fb.GetHandle());
				math::Vector<2, unsigned> size(fb.GetSize());
				glViewport(0, 0, size.x, size.y);
			}
			void ResetFramebufferBinding(const DrawContext &dc)
			{
				glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
				math::Vector<2, unsigned> size(dc.GetDriver().GetWindow().GetSize());
				glViewport(0, 0, size.x, size.y);
			}
		}
	}
}
