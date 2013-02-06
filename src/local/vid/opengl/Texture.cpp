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

#include <algorithm> // transform
#include <cassert>
#include "../../cfg/vars.hpp"
#include "../../math/Color.hpp" // RgbaColor
#include "../../math/pow2.hpp" // Pow2Ceil
#include "ext.hpp" // ARB_texture_non_power_of_two
#include "tex.hpp" // Make{,Alpha,Luminance}Texture
#include "Texture.hpp"

namespace page
{
	namespace vid
	{
		namespace opengl
		{
			// construct/destroy
			Texture::Texture(const math::RgbaColor<> &color) :
				size(1), pow2Size(1)
			{
				glGenTextures(1, &handle);
				glBindTexture(GL_TEXTURE_2D, handle);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA,
					GL_FLOAT, &*math::RgbaColor<GLfloat>(color).begin());
			}
			Texture::Texture(const res::Image &img, Format format, Flags flags, const math::Vector<2, bool> &clamp) :
				size(img.size), pow2Size(Max(size >> CVAR(opengl)::renderTextureDown, 1))
			{
				// FIXME: texture size calculations are duplicated in tex.cpp
				if (!haveArbTextureNonPowerOfTwo)
					std::transform(size.begin(), size.end(), pow2Size.begin(), math::Pow2Ceil);
				// FIXME: Make*Texture should accept flags directly
				bool mipmap = flags & mipmapTextureFlag;
				switch (format)
				{
					case defaultTextureFormat:   handle = MakeTexture(         img, mipmap, clamp); break;
					case alphaTextureFormat:     handle = MakeAlphaTexture(    img, mipmap, clamp); break;
					case luminanceTextureFormat: handle = MakeLuminanceTexture(img, mipmap, clamp); break;
					default: assert(!"invalid texture format");
				}
			}
			Texture::~Texture()
			{
				glDeleteTextures(1, &handle);
			}

			// attributes
			const math::Vector<2, unsigned> &Texture::GetSize() const
			{
				return size;
			}
			const math::Vector<2, unsigned> &Texture::GetPow2Size() const
			{
				return pow2Size;
			}

			// handle access
			GLuint Texture::GetHandle() const
			{
				return handle;
			}

			// binding
			void Bind(const Texture &texture)
			{
				glBindTexture(GL_TEXTURE_2D, texture.GetHandle());
				glEnable(GL_TEXTURE_2D);
			}
		}
	}
}
