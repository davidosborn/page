#include "../../../vid/opengl/FontTexture.hpp"
#include "FontTextureProxy.hpp"

namespace page { namespace cache { namespace opengl
{
	/*-------------+
	| constructors |
	+-------------*/

	FontTextureProxy::FontTextureProxy(const Proxy<res::Font> &font, unsigned fontSize) :
		BasicProxy<vid::opengl::FontTexture>(Signature("OpenGL font texture", font, fontSize)),
		font(font), fontSize(fontSize) {}

	/*--------------------------+
	| BasicProxy implementation |
	+--------------------------*/

	auto FontTextureProxy::DoLock() const -> pointer
	{
		return pointer(new vid::opengl::FontTexture(*font, fontSize));
	}
}}}
