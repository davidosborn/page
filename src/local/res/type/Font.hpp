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

#ifndef    page_local_res_type_Font_hpp
#   define page_local_res_type_Font_hpp

#	include <string>
#	include <unordered_map>
#	include <vector>

#	include "../../math/Vector.hpp"
#	include "Image.hpp"

namespace page
{
	namespace res
	{
		namespace detail
		{
			struct KerningHash : std::unary_function<std::pair<char, char>, std::size_t>
			{
				std::size_t operator ()(const std::pair<char, char> &) const;
			};
		}

		struct Font
		{
			struct Glyph
			{
				struct Outline
				{
					struct Contour
					{
						struct Point
						{
							math::Vector<2> co;
							enum Type
							{
								bezier2Type,
								bezier3Type,
								controlType
							} type;
						};
						typedef std::vector<Point> Points;
						Points points;
					};
					typedef std::vector<Contour> Contours;
					Contours contours;
				} outline;
				typedef std::vector<Image> Renditions;
				Renditions renditions;
				math::Vector<2> size, bearing;
				float advance;
			};
			typedef std::unordered_map<char, Glyph> Glyphs;
			Glyphs glyphs;
			typedef std::unordered_map<std::pair<char, char>, float, detail::KerningHash> Kernings;
			Kernings kernings;
			math::Vector<2> maxSize, maxBearing;
			float maxAdvance, lineHeight;
		};

		// text alignment
		enum TextAlign
		{
			leftTextAlign,
			centerTextAlign,
			rightTextAlign,
			justifyTextAlign
		};

		// glyph access
		const Font::Glyph *GetGlyph(const Font &, char);

		// glyph images
		Image GetCharImage(const Font &, char, float fontSize);
		Image RenderOutline(const Font::Glyph &, float fontSize);

		// advance
		float GetAdvance(const Font &, char);
		float GetAdvance(const Font &, char, char);

		// line wrapping
		std::string Wrap(const Font &, std::string, float width);

		// line width calculations
		float GetLineWidth(const Font &,
			const std::string::const_iterator &first,
			const std::string::const_iterator &last);
		float GetCenteredOffset(const Font &,
			const std::string::const_iterator &first,
			const std::string::const_iterator &last, float width);
		float GetRightAlignedOffset(const Font &,
			const std::string::const_iterator &first,
			const std::string::const_iterator &last, float width);
		float GetJustifiedScale(const Font &,
			const std::string::const_iterator &first,
			const std::string::const_iterator &last, float width);

		// text size calculations
		float GetMaxLineWidth(const Font &,
			const std::string::const_iterator &first,
			const std::string::const_iterator &last);
		unsigned CountLines(const Font &,
			const std::string::const_iterator &first,
			const std::string::const_iterator &last, float width = 0);
		math::Vector<2> GetTextSize(const Font &,
			const std::string::const_iterator &first,
			const std::string::const_iterator &last, float width = 0);
	}
}

#endif
