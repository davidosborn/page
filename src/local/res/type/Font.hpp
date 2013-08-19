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
							math::Vec2 co;
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
				math::Vec2 size, bearing;
				float advance;
			};
			typedef std::unordered_map<char, Glyph> Glyphs;
			Glyphs glyphs;
			typedef std::unordered_map<std::pair<char, char>, float, detail::KerningHash> Kernings;
			Kernings kernings;
			math::Vec2 maxSize, maxBearing;
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
		math::Vec2 GetTextSize(const Font &,
			const std::string::const_iterator &first,
			const std::string::const_iterator &last, float width = 0);
	}
}

#endif
