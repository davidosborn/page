/**
 * @copyright
 *
 * Copyright (c) 2006-2014 David Osborn
 *
 * Permission is granted to use and redistribute this software in source and
 * binary form, with or without modification, subject to the following
 * conditions:
 *
 * 1. Redistributions in source form must retain the above copyright notice,
 *    this list of conditions, and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions, and the following disclaimer in the same place
 *    and form as other copyright, license, and disclaimer information.
 *
 * 3. Redistributions in binary form must also include an acknowledgement in the
 *    same place and form as other acknowledgements (such as the credits),
 *    similar in substance to the following:
 *
 *       Portions of this software are based on the work of David Osborn.
 *
 * This software is provided "as is", without any express or implied warranty.
 * In no event will the authors be liable for any damages arising out of the use
 * of this software.
 */

#include <cassert>
#include <memory> // {shared,unique}_ptr
#include <unordered_map>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "../../../err/Exception.hpp"
#include "../../../util/cpp.hpp" // STRINGIZE
#include "../../adapt/freetype.hpp" // FracToFloat, GetLib, OpenArgs
#include "../../type/Font.hpp"
#include "../LoaderRegistry.hpp" // REGISTER_LOADER
#include "freetype.hpp" // LoadFreetypeFont

namespace page { namespace res
{
	namespace
	{
		std::unique_ptr<Font> LoadFreetypeFont(const OpenArgs &args, unsigned faceIndex)
		{
			FT_Face face;
			if (FT_Open_Face(GetLib(), args.Get(), faceIndex, &face)) return 0;
			const std::unique_ptr<Font> font(new Font);
			try
			{
				// map existing characters to glyphs
				typedef std::unordered_map<char, unsigned> CharGlyphs;
				CharGlyphs charGlyphs;
				charGlyphs.insert(std::make_pair(0, 0));
				for (unsigned char code = 32; code < 127; ++code) // ASCII
				{
					FT_UInt glyph = FT_Get_Char_Index(face, code);
					if (glyph) charGlyphs.insert(std::make_pair(code, glyph));
				}
				// load glyphs
				for (CharGlyphs::const_iterator iter(charGlyphs.begin()); iter != charGlyphs.end(); ++iter)
				{
					Font::Glyph glyph;
					unsigned metricResolution = 0;
					// load outline
					if (FT_IS_SCALABLE(face))
					{
						if (FT_Load_Glyph(face, iter->second, FT_LOAD_NO_SCALE))
							THROW((err::Exception<err::ResModuleTag, err::FreetypePlatformTag>("failed to load glyph outline")))
						if (face->glyph->format == FT_GLYPH_FORMAT_OUTLINE)
						{
							// load contours
							const FT_Outline &ftOutline(face->glyph->outline);
							unsigned ftPointIndex = 0;
							for (short *ftContour = ftOutline.contours; ftContour != ftOutline.contours + ftOutline.n_contours; ++ftContour)
							{
								Font::Glyph::Outline::Contour contour;
								for (; ftPointIndex <= *ftContour; ++ftPointIndex)
								{
									Font::Glyph::Outline::Contour::Point point =
									{
										(math::Vec2(
											ftOutline.points[ftPointIndex].x,
											ftOutline.points[ftPointIndex].y) - math::Vec2(
											face->glyph->metrics.horiBearingX,
											face->glyph->metrics.horiBearingY - face->glyph->metrics.height)) / face->units_per_EM
									};
									switch (ftOutline.tags[ftPointIndex])
									{
										case 0: point.type = Font::Glyph::Outline::Contour::Point::bezier2Type; break;
										case 1: point.type = Font::Glyph::Outline::Contour::Point::controlType; break;
										case 2: point.type = Font::Glyph::Outline::Contour::Point::bezier3Type; break;
										default: assert(!"invalid point tag");
									}
									contour.points.push_back(point);
								}
								glyph.outline.contours.push_back(contour);
							}
							// load glyph metrics
							const FT_Glyph_Metrics &metrics(face->glyph->metrics);
							glyph.size = math::Vec2(
								metrics.width,
								metrics.height) / face->units_per_EM;
							glyph.bearing = math::Vec2(
								metrics.horiBearingX,
								metrics.horiBearingY) / face->units_per_EM;
							glyph.advance = float(metrics.horiAdvance) / face->units_per_EM;
							metricResolution = face->units_per_EM;
						}
					}
					// load renditions
					if (FT_HAS_FIXED_SIZES(face))
					{
						for (unsigned i = 0; i < face->num_fixed_sizes; ++i)
						{
							if (FT_Select_Size(face, i))
								THROW((err::Exception<err::ResModuleTag, err::FreetypePlatformTag>("failed to select bitmap strike")))
							if (FT_Load_Glyph(face, iter->second, FT_LOAD_DEFAULT))
								THROW((err::Exception<err::ResModuleTag, err::FreetypePlatformTag>("failed to load glyph bitmap")))
							if (face->glyph->format == FT_GLYPH_FORMAT_BITMAP)
							{
								// FIXME: implement; load renditions
								// load glyph metrics
								FT_Int ppem = face->size->metrics.y_ppem;
								if (ppem > metricResolution)
								{
									const FT_Glyph_Metrics &metrics(face->glyph->metrics);
									glyph.size = math::Vec2(
										FracToFloat(metrics.width),
										FracToFloat(metrics.height)) / ppem;
									glyph.bearing = math::Vec2(
										FracToFloat(metrics.horiBearingX),
										FracToFloat(metrics.horiBearingY)) / ppem;
									glyph.advance = FracToFloat(metrics.horiAdvance) / ppem;
									metricResolution = ppem;
								}
							}
						}
					}
					font->glyphs.insert(std::make_pair(iter->first, glyph));
				}
				// load kernings
				if (FT_HAS_KERNING(face))
				{
					for (CharGlyphs::const_iterator left(charGlyphs.begin()); left != charGlyphs.end(); ++left)
						for (CharGlyphs::const_iterator right(charGlyphs.begin()); right != charGlyphs.end(); ++right)
						{
							FT_Vector vector;
							if (FT_Get_Kerning(face, left->second, right->second, FT_KERNING_UNSCALED, &vector))
								THROW((err::Exception<err::ResModuleTag, err::FreetypePlatformTag>("failed to load kerning")))
							// FIXME: need to throw away kernings which are the
							// same as the default kerning
							if (FT_IS_SCALABLE(face))
							{
								// FIXME: implement; vector is in font units
							}
							else
							{
								// FIXME: implement; vector is in pixels
							}
						}
				}
				// load font metrics
				if (FT_IS_SCALABLE(face))
				{
					font->maxAdvance = float(face->max_advance_width) / face->units_per_EM;
					font->lineHeight = float(face->height) / face->units_per_EM;
				}
				else
				{
					// use the bitmap strike with the highest resolution
					const FT_Bitmap_Size *metrics = face->available_sizes;
					for (FT_Int i = 0; i < face->num_fixed_sizes; ++i)
						if (face->available_sizes[i].y_ppem > metrics->y_ppem)
							metrics = face->available_sizes + i;
					font->maxAdvance = FracToFloat(metrics->x_ppem) / metrics->y_ppem;
					font->lineHeight = FracToFloat(metrics->height) / metrics->y_ppem;
				}
			}
			catch (...)
			{
				FT_Done_Face(face);
				throw;
			}
			FT_Done_Face(face);
			// calculate maximum glyph metrics
			for (Font::Glyphs::const_iterator iter(font->glyphs.begin()); iter != font->glyphs.end(); ++iter)
			{
				const Font::Glyph &glyph(iter->second);
				font->maxSize    = Max(glyph.size,    font->maxSize);
				font->maxBearing = Max(glyph.bearing, font->maxBearing);
			}
			return font.release();
		}
	}

	std::unique_ptr<Font> LoadFreetypeFont(const std::shared_ptr<const Pipe> &pipe)
	{
		assert(pipe);
		OpenArgs args(*pipe);
		return LoadFreetypeFont(args);
	}

	bool CheckFreetypeFont(const Pipe &pipe)
	{
		OpenArgs args(pipe);
		return !FT_Open_Face(GetLib(), args.Get(), -1, 0);
	}

	REGISTER_LOADER(
		Font,
		STRINGIZE(NAME) " font",
		LoadFreetypeFont,
		CheckFreetypeFont,
		{}, // NOTE: there are currently no font mime types
		{"bdf", "cff", "fnt", "fon", "otf", "pcf", "pfa", "pfb", "pfr", "ttc", "ttf"})
}}
