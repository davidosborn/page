#include "../../../util/string/StringBuilder.hpp"
#include "../../../vid/opengl/FontTexture.hpp"
#include "FontTexture.hpp"

namespace page { namespace cache { namespace opengl
{
	/*--------------------------+
	| constructors & destructor |
	+--------------------------*/

	FontTexture::FontTexture(const Proxy<res::Font> &font, unsigned fontSize) :
		font(font.Copy()), fontSize(fontSize) {}

	/*----------+
	| observers |
	+----------*/

	std::string FontTexture::GetType() const
	{
		return "font texture";
	}

	std::string FontTexture::GetSource() const
	{
		return util::StringBuilder() <<
			font->GetSource() << ':' << fontSize;
	}

	FontTexture::operator bool() const
	{
		return *font;
	}

	/*--------------+
	| instantiation |
	+--------------*/

	FontTexture::Instance FontTexture::Make() const
	{
		return Instance(new vid::opengl::FontTexture(**font, fontSize));
	}
}}}
