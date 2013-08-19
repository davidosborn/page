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

				math::Vec2 offset;
				bool scale;
			};
			struct Frame
			{
				typedef Component Side;
				Side left, top, right, bottom;
				struct Corner : Component
				{
					math::Vec2 offset;
				} topLeft, topRight, bottomLeft, bottomRight;
			};
			struct Decoration : Component
			{
				Decoration();

				bool background;
				math::Vec2 center, position, offset;
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
