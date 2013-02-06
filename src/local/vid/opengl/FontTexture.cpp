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
				math::Vector<2, unsigned> texSize;
				math::Vector<2, unsigned> glyphSize(Ceil(font.maxSize * fontSize) + 1);
				std::transform(glyphSize.begin(), glyphSize.end(), texSize.begin(), math::Pow2Ceil);
				while (Content(texSize / glyphSize) < font.glyphs.size())
					if (texSize.x < texSize.y) texSize.x *= 2;
					else if (texSize.x > texSize.y) texSize.y *= 2;
					else texSize[
						Content(math::Vector<2, unsigned>(texSize.x * 2, texSize.y) / glyphSize) <
						Content(math::Vector<2, unsigned>(texSize.x, texSize.y * 2) / glyphSize)] *= 2;
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
				math::Vector<2, unsigned> pos;
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
					math::Vector<2>
						min(math::Vector<2>(pos) / texSize),
						max(min + math::Vector<2>(img.size) / texSize);
					sections.insert(std::make_pair(iter->first, math::Aabb<2>(min, max)));
					// increment insertion position
					if ((pos.x += glyphSize.x) > texSize.x - glyphSize.x)
						pos = math::Vector<2, unsigned>(0, pos.y + glyphSize.y);
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
