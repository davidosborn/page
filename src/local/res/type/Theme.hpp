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

#ifndef    page_local_res_type_Theme_hpp
#   define page_local_res_type_Theme_hpp

#	include "../../cache/proxy/Resource.hpp"
#	include "../../math/Aabb.hpp"
#	include "../../math/Color.hpp" // RgbaColor
#	include "../../math/Vector.hpp"

namespace page
{
	namespace res
	{
		class Cursor;
		class Font;
		class Image;

		struct Theme
		{
			Theme();

			cache::Resource<Cursor> cursor;
			float margin, scale;
			struct Component
			{
				cache::Resource<Image> image;
				math::Vector<2, bool> flip;
			};
			struct Background : Component
			{
				Background();

				math::Vector<2> offset;
				bool scale;
			};
			struct Frame
			{
				typedef Component Side;
				Side left, top, right, bottom;
				struct Corner : Component
				{
					math::Vector<2> offset;
				} topLeft, topRight, bottomLeft, bottomRight;
			};
			struct Decoration : Component
			{
				Decoration();

				bool background;
				math::Vector<2> center, position, offset;
				float angle;
			};
			typedef std::vector<Decoration> Decorations;
			struct Panel
			{
				Panel();

				float margin;
				Background background;
				Frame frame;
				Decorations decorations;
			};
			struct Text
			{
				Text();

				cache::Resource<Font> font;
				float size;
			} text;
			struct Window : Panel
			{
				struct Title
				{
					Background background;
					Text text;
				} title;
			} window;
			struct Button : Panel
			{
				Text text;
			} button;
			struct Edit : Panel
			{
				Text text;
			} edit;
			struct List : Panel
			{
				Text text;
				Component separator;
				Background highlight;
			} list;
		};

		// metrics
		// NOTE: AABB misappropriated to return thickness of sides
		math::Aabb<2> GetThickness(const Theme::Frame &);

		// defaults
		void SetDefaults(Theme &);
	}
}

#endif
