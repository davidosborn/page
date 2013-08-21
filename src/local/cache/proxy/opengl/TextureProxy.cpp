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
		BasicProxy(MakeSignature(image, format, flags, clamp)),
		image(image), format(format), flags(flags), clamp(clamp) {}

	std::string TextureProxy::MakeSignature(const Proxy<res::Image> &image, vid::opengl::TextureFormat format, vid::opengl::TextureFlags flags, const math::Vector<2, bool> &clamp)
	{
		std::ostringstream ss;
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
		return std::make_shared<vid::opengl::Texture>(**image, format, flags, clamp);
	}
}}}
