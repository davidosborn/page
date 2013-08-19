#include <algorithm> // copy_if, count, find{,_if_not}
#include <cassert>
#include <cctype> // isspace
#include <climits> // UCHAR_MAX
#include <cmath> // ceil
#include <functional> // hash
#include <iterator> // back_inserter

#include "../../util/functional/locale.hpp" // isspace_function
#include "Font.hpp"
#include "Registry.hpp" // REGISTER_TYPE

namespace page
{
	namespace res
	{
		namespace detail
		{
			std::size_t KerningHash::operator ()(const std::pair<char, char> &pair) const
			{
				return std::hash<std::size_t>()(pair.first + pair.second * UCHAR_MAX);
			}
		}

		// glyph access
		const Font::Glyph *GetGlyph(const Font &font, char c)
		{
			Font::Glyphs::const_iterator iter(font.glyphs.find(c));
			if (iter == font.glyphs.end() && (std::isspace(c) ||
				(iter = font.glyphs.find('\0')) == font.glyphs.end())) return 0;
			return &iter->second;
		}

		// glyph images
		Image GetCharImage(const Font &font, char c, float fontSize)
		{
			assert(fontSize);
			if (const Font::Glyph *glyph = GetGlyph(font, c))
			{
				unsigned size = std::ceil(glyph->size.y * fontSize);
				// try the closest matching rendition
				Font::Glyph::Renditions::const_iterator bestRendition(glyph->renditions.end());
				for (Font::Glyph::Renditions::const_iterator rendition(glyph->renditions.begin()); rendition != glyph->renditions.end(); ++rendition)
					if (rendition->size.y >= size && (bestRendition == glyph->renditions.end() || rendition->size.y < bestRendition->size.y))
						bestRendition = rendition;
				if (bestRendition != glyph->renditions.end())
					return *bestRendition;
				// try the rendered outline
				if (!glyph->outline.contours.empty())
					return RenderOutline(*glyph, fontSize);
				// try the largest available rendition
				for (Font::Glyph::Renditions::const_iterator rendition(glyph->renditions.begin()); rendition != glyph->renditions.end(); ++rendition)
					if (bestRendition == glyph->renditions.end() || rendition->size.y > bestRendition->size.y)
						bestRendition = rendition;
				if (bestRendition != glyph->renditions.end())
					return *bestRendition;
			}
			return c ? GetCharImage(font, 0, fontSize) : Image();
		}
#ifndef USE_FREETYPE
		Image RenderOutline(const Font::Glyph &glyph, float fontSize)
		{
			assert(fontSize);
			// FIXME: implement
			return Image();
		}
#endif

		// advance
		float GetAdvance(const Font &font, char c)
		{
			const Font::Glyph *glyph = GetGlyph(font, c);
			return glyph ? glyph->advance : 1;
		}
		float GetAdvance(const Font &font, char first, char second)
		{
			Font::Kernings::const_iterator kerning(
				font.kernings.find(std::make_pair(first, second)));
			return kerning != font.kernings.end() ? kerning->second :
				GetAdvance(font, first);
		}

		// line wrapping
		std::string Wrap(const Font &font, std::string s, float width)
		{
			float offset = 0, hardOffset = 0;
			std::string::iterator space(s.end());
			char lastChar = 0;
			for (std::string::iterator c(s.begin()); c != s.end(); ++c)
			{
				if (*c == '\n')
				{
					offset = 0;
					space = s.end();
					continue;
				}
				if (std::isspace(*c) && !std::isspace(lastChar))
				{
					hardOffset = offset;
					space = c;
				}
				std::string::const_iterator next(c + 1);
				offset += next != s.end() ?
					GetAdvance(font, *c, *next) :
					GetAdvance(font, *c);
				if (offset > width && space != s.end())
				{
					std::string::iterator endSpace(std::find_if_not(space, s.end(), util::isspace_function<char>()));
					std::string::iterator::difference_type index = c - s.begin() - (endSpace - space - 1);
					s.replace(space, endSpace, 1, '\n');
					c = s.begin() + index;
					offset -= hardOffset;
					space = s.end();
				}
				lastChar = *c;
			}
			return s;
		}

		// line width calculations
		float GetLineWidth(const Font &font,
			const std::string::const_iterator &first,
			const std::string::const_iterator &last)
		{
			float width = 0;
			for (std::string::const_iterator c(first); c != last; ++c)
			{
				assert(*c != '\n');
				std::string::const_iterator next(c + 1);
				width += next != last ?
					GetAdvance(font, *c, *next) :
					GetAdvance(font, *c);
			}
			return width;
		}
		float GetCenteredOffset(const Font &font,
			const std::string::const_iterator &first,
			const std::string::const_iterator &last, float width)
		{
			return GetRightAlignedOffset(font, first, last, width) / 2;
		}
		float GetRightAlignedOffset(const Font &font,
			const std::string::const_iterator &first,
			const std::string::const_iterator &last, float width)
		{
			return width - GetLineWidth(font, first, last);
		}
		float GetJustifiedScale(const Font &font,
			const std::string::const_iterator &first,
			const std::string::const_iterator &last, float width)
		{
			std::string whitespace;
			std::copy_if(first, last, std::back_inserter(whitespace), util::isspace_function<char>());
			float
				remainder = width - GetLineWidth(font, first, last),
				spaceWidth = GetLineWidth(font, whitespace.begin(), whitespace.end());
			return spaceWidth ? (spaceWidth + remainder) / spaceWidth : 0;
		}

		// text size calculations
		float GetMaxLineWidth(const Font &font,
			const std::string::const_iterator &first,
			const std::string::const_iterator &last)
		{
			float width = 0;
			for (std::string::const_iterator c(first); c != last; ++c)
			{
				std::string::const_iterator end(std::find(c, last, '\n'));
				width = std::max(GetLineWidth(font, c, end), width);
				if ((c = end) == last) break;
			}
			return width;
		}
		unsigned CountLines(const Font &font,
			const std::string::const_iterator &first,
			const std::string::const_iterator &last, float width)
		{
			if (width)
			{
				std::string text(Wrap(font, std::string(first, last), width));
				return std::count(text.begin(), text.end(), '\n') + 1;
			}
			return std::count(first, last, '\n') + 1;
		}
		math::Vec2 GetTextSize(const Font &font,
			const std::string::const_iterator &first,
			const std::string::const_iterator &last, float width)
		{
			return math::Vec2(
				width ? width : GetMaxLineWidth(font, first, last),
				font.lineHeight * CountLines(font, first, last, width));
		}

		REGISTER_TYPE(Font, "font", 0)
	}
}
