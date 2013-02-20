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

#include <cassert>

#include "../../../vid/opengl/Texture.hpp"
#include "Texture.hpp"

namespace page
{
	namespace cache
	{
		namespace opengl
		{
			// construct
			Texture::Texture(const Proxy<res::Image> &image, vid::opengl::TextureFormat format, vid::opengl::TextureFlags flags, const math::Vector<2, bool> &clamp) :
				image(image.Copy()), format(format), flags(flags), clamp(clamp) {}

			// clone
			Texture *Texture::Clone() const
			{
				return new Texture(*this);
			}

			// attributes
			std::string Texture::GetType() const
			{
				return "texture";
			}
			std::string Texture::GetSource() const
			{
				std::string source(image->GetSource());
				switch (format)
				{
					case vid::opengl::defaultTextureFormat: break;
					case vid::opengl::alphaTextureFormat:     source += ":alpha";     break;
					case vid::opengl::luminanceTextureFormat: source += ":luminance"; break;
					default: assert(!"invalid texture format");
				}
				if (flags & vid::opengl::filterTextureFlag) source += ":filter";
				if (flags & vid::opengl::mipmapTextureFlag) source += ":mipmap";
				if (Any(clamp))
				{
					source += ":clamp";
					if      (!clamp.x) source += ".y";
					else if (!clamp.y) source += ".x";
				}
				return source;
			}

			// dependency satisfaction
			Texture::operator bool() const
			{
				return *image;
			}

			// instantiation
			Texture::Instance Texture::Make() const
			{
				return Instance(new vid::opengl::Texture(**image, format, flags, clamp));
			}
		}
	}
}
