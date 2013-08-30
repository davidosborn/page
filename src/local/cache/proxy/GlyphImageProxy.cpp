#include "../../res/type/Font.hpp" // GetCharImage
#include "GlyphImageProxy.hpp"

namespace page { namespace cache
{
	/*-------------+
	| constructors |
	+-------------*/

	GlyphImageProxy::GlyphImageProxy(const Proxy<res::Font> &font, char ch, unsigned size) :
		BasicProxy<res::Image>(Signature("glyph image", font, ch, size)),
		font(font), ch(ch), size(size) {}

	/*--------------------------+
	| BasicProxy implementation |
	+--------------------------*/

	auto GlyphImageProxy::DoLock() const -> pointer
	{
		return std::make_shared<res::Image>(GetCharImage(**font, ch, size));
	}
}}
