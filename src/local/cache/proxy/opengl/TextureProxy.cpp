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

#include <cassert>
#include <sstream> // ostringstream

#include "../../../vid/opengl/Texture.hpp"
#include "Texture.hpp"

namespace page { namespace cache { namespace opengl
{
	/*-------------+
	| constructors |
	+-------------*/

	TextureProxy::TextureProxy(const Proxy<res::Image> &image, vid::opengl::TextureFormat format, vid::opengl::TextureFlags flags, const math::Vector<2, bool> &clamp) :
		BasicProxy<vid::opengl::Texture>(MakeSignature(image, format, flags, clamp)),
		image(image), format(format), flags(flags), clamp(clamp) {}

	Signature TextureProxy::MakeSignature(const Proxy<res::Image> &image, vid::opengl::TextureFormat format, vid::opengl::TextureFlags flags, const math::Vector<2, bool> &clamp)
	{
		std::ostringstream ss(image.GetSignature());
		switch (format)
		{
			case vid::opengl::defaultTextureFormat: break;
			case vid::opengl::alphaTextureFormat:     ss << ":alpha";     break;
			case vid::opengl::luminanceTextureFormat: ss << ":luminance"; break;
			default: assert(!"invalid texture format");
		}
		if (flags & vid::opengl::filterTextureFlag) ss << ":filter";
		if (flags & vid::opengl::mipmapTextureFlag) ss << ":mipmap";
		if (Any(clamp))
		{
			ss << ":clamp";
			if      (!clamp.x) ss << ".y";
			else if (!clamp.y) ss << ".x";
		}
		return Signature("OpenGL texture", ss.str());
	}

	/*--------------------------+
	| BasicProxy implementation |
	+--------------------------*/

	auto TextureProxy::DoLock() const -> pointer
	{
		return std::make_shared<vid::opengl::Texture>(*image, format, flags, clamp);
	}
}}}
