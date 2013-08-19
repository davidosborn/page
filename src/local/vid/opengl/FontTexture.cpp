#include <algorithm> // transform
#include <cctype> // isspace

#include "../../err/Exception.hpp"
#include "../../math/pow2.hpp" // Pow2Ceil
#include "../../res/type/Font.hpp"
#include "ext.hpp" // ARB_texture_non_power_of_two
#include "FontTexture.hpp"
#include "tex.hpp" // Compatibility, GetCompatibility

namespace page
{
	namespace vid
	{
		namespace opengl
		{
			// construct/destroy
			FontTexture::FontTexture(const res::Font &font, unsigned fontSize)
			{
				// calculate best fitting texture size
				math::Vec2u texSize;
				math::Vec2u glyphSize(Ceil(font.maxSize * fontSize) + 1);
				std::transform(glyphSize.begin(), glyphSize.end(), texSize.begin(), math::Pow2Ceil);
				while (Content(texSize / glyphSize) < font.glyphs.size())
					if (texSize.x < texSize.y) texSize.x *= 2;
					else if (texSize.x > texSize.y) texSize.y *= 2;
					else texSize[
						Content(math::Vec2u(texSize.x * 2, texSize.y) / glyphSize) <
						Content(math::Vec2u(texSize.x, texSize.y * 2) / glyphSize)] *= 2;
				if (haveArbTextureNonPowerOfTwo)
					texSize -= texSize % glyphSize;
				// generate texture
				if (glGenTextures(1, &handle), glGetError())
					THROW((err::Exception<err::VidModuleTag, err::OpenglPlatformTag>("failed to generate texture")))
				glBindTexture(GL_TEXTURE_2D, handle);
				// FIXME: add support for other formats
				std::vector<GLubyte> data(Content(texSize));
				glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, texSize.x, texSize.y, 0, GL_ALPHA, GL_UNSIGNED_BYTE, &*data.begin());
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				if (glGetError())
					THROW((err::Exception<err::VidModuleTag, err::OpenglPlatformTag>("failed to initialize texture")))
				// insert glyphs
				math::Vec2u pos;
				for (res::Font::Glyphs::const_iterator iter(font.glyphs.begin()); iter != font.glyphs.end(); ++iter)
				{
					if (std::isspace(iter->first)) continue;
					// convert glyph to compatible format
					res::Image img(GetCharImage(font, iter->first, fontSize));
					Compatibility compat(GetCompatibility(img));
					if (!compat.channels.empty()) img = Convert(img, compat.channels, 1);
					else if (!IsAligned(img, 1)) img = Align(img, 1);
					// insert texture
					glTexSubImage2D(GL_TEXTURE_2D, 0, pos.x, pos.y, img.size.x, img.size.y, compat.format, compat.type, &*img.data.begin());
					// insert section
					math::Vec2
						min(math::Vec2(pos) / texSize),
						max(min + math::Vec2(img.size) / texSize);
					sections.insert(std::make_pair(iter->first, math::Aabb<2>(min, max)));
					// increment insertion position
					if ((pos.x += glyphSize.x) > texSize.x - glyphSize.x)
						pos = math::Vec2u(0, pos.y + glyphSize.y);
				}
			}
			FontTexture::~FontTexture()
			{
				glDeleteTextures(1, &handle);
			}

			// sections
			const math::Aabb<2> *FontTexture::GetSection(char c) const
			{
				Sections::const_iterator iter(sections.find(c));
				return iter != sections.end() ||
					(iter = sections.find(0)) != sections.end() ?
					&iter->second : 0;
			}

			// handle access
			GLuint FontTexture::GetHandle() const
			{
				return handle;
			}

			// binding
			void Bind(const FontTexture &texture)
			{
				glBindTexture(GL_TEXTURE_2D, texture.GetHandle());
				glEnable(GL_TEXTURE_2D);
			}
		}
	}
}
