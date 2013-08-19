#include <cassert>
#include <sstream> // ostringstream

#include "../../../vid/opengl/Texture.hpp"
#include "Texture.hpp"

namespace page { namespace cache { namespace opengl
{
	/*--------------------------+
	| constructors & destructor |
	+--------------------------*/

	Texture::Texture(const Proxy<res::Image> &image, vid::opengl::TextureFormat format, vid::opengl::TextureFlags flags, const math::Vector<2, bool> &clamp) :
		image(image.Copy()), format(format), flags(flags), clamp(clamp) {}

	/*----------+
	| observers |
	+----------*/

	std::string Texture::GetType() const
	{
		return "texture";
	}

	std::string Texture::GetSource() const
	{
		std::ostringstream ss(image->GetSource());
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
		return ss.str();
	}

	Texture::operator bool() const
	{
		return *image;
	}

	/*--------------+
	| instantiation |
	+--------------*/

	Texture::Instance Texture::Make() const
	{
		return std::make_shared<vid::opengl::Texture>(**image, format, flags, clamp);
	}
}}}
