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

#include <cassert>
#include <climits> // CHAR_BIT
#include <vector>
#include <ft2build.h>
#include FT_OUTLINE_H
#include "../../../err/Exception.hpp"
#include "../../adapt/freetype.hpp" // FloatToFrac, GetLib
#include "../Font.hpp" // Font::Glyph::Outline

namespace page
{
	namespace res
	{
		Image RenderOutline(const Font::Glyph &glyph, float fontSize)
		{
			assert(fontSize);
			// initialize destination image
			Image img;
			img.size = math::Vector<2, unsigned>(Ceil(glyph.size * fontSize));
			img.channels.push_back(Image::Channel(Image::Channel::alpha, 1));
			img.alignment = 1;
			if (!All(img.size)) return img;
			img.data.resize((img.size.x + CHAR_BIT - 1) / CHAR_BIT * img.size.y);
			// initialize FreeType bitmap
			FT_Bitmap ftBitmap =
			{
				img.size.y,
				img.size.x,
				(img.size.x + CHAR_BIT - 1) / CHAR_BIT,
				&*img.data.begin(),
				0, FT_PIXEL_MODE_MONO
			};
			// initialize FreeType outline
			std::vector<FT_Vector> ftPoints;
			std::vector<char> ftTags;
			std::vector<short> ftContours;
			ftContours.reserve(glyph.outline.contours.size());
			math::Vector<2> scale(math::Vector<2>(img.size) / glyph.size);
			for (Font::Glyph::Outline::Contours::const_iterator contour(glyph.outline.contours.begin()); contour != glyph.outline.contours.end(); ++contour)
			{
				ftPoints.reserve(ftPoints.size() + contour->points.size());
				ftTags.reserve(ftTags.size() + contour->points.size());
				for (Font::Glyph::Outline::Contour::Points::const_iterator point(contour->points.begin()); point != contour->points.end(); ++point)
				{
					FT_Vector ftPoint =
					{
						FloatToFrac(point->co.x * scale.x),
						FloatToFrac(point->co.y * scale.y)
					};
					ftPoints.push_back(ftPoint);
					char ftTag;
					switch (point->type)
					{
						case Font::Glyph::Outline::Contour::Point::bezier2Type: ftTag = 0; break;
						case Font::Glyph::Outline::Contour::Point::bezier3Type: ftTag = 2; break;
						case Font::Glyph::Outline::Contour::Point::controlType: ftTag = 1; break;
						default: assert(!"invalid point type");
					}
					ftTags.push_back(ftTag);
				}
				ftContours.push_back(ftPoints.size() - 1);
			}
			FT_Outline ftOutline =
			{
				static_cast<short>(ftContours.size()),
				static_cast<short>(ftPoints.size()),
				&*ftPoints.begin(),
				&*ftTags.begin(),
				&*ftContours.begin(),
				FT_OUTLINE_NONE
			};
			// set raster parameters
			FT_Raster_Params rasterParams =
			{
				&ftBitmap,
				&ftOutline,
				FT_RASTER_FLAG_DEFAULT
			};
			if (FT_Outline_Render(GetLib(), &ftOutline, &rasterParams))
				THROW((err::Exception<err::ResModuleTag, err::FreetypePlatformTag>("failed to render outline")))
			return img;
		}
	}
}
