#include "Image.hpp" // Image::size
#include "Registry.hpp" // REGISTER_TYPE
#include "Theme.hpp"

namespace page
{
	namespace res
	{
		// construct
		Theme::Theme() : margin(16), scale(1024) {}
		Theme::Background::Background() : scale(true) {}
		Theme::Decoration::Decoration() : background(false), center(.5), angle(0) {}
		Theme::Panel::Panel() : margin(0) {}
		Theme::Text::Text() : size(0) {}

		// metrics
		math::Aabb<2> GetThickness(const Theme::Frame &frame)
		{
			return math::Aabb<2>(
				math::Vec2(
					frame.left.image   ? frame.left.image->size.y   : 0,
					frame.top.image    ? frame.top.image->size.y    : 0),
				math::Vec2(
					frame.right.image  ? frame.right.image->size.y  : 0,
					frame.bottom.image ? frame.bottom.image->size.y : 0));
		}

		// defaults
		void SetDefaults(Theme &theme)
		{
			// FIXME: panel margin of zero is defaulting to theme margin,
			// but we may need a panel margin of zero at some point
			if (!theme.text.font)
				theme.text.font = cache::ResourceProxy<Font>("font/default.ttf");
			if (!theme.text.size)
				theme.text.size = 16;
			if (!theme.window.margin)
				theme.window.margin = theme.margin;
			if (!theme.window.title.text.font)
				theme.window.title.text.font = theme.text.font;
			if (!theme.window.title.text.size)
				theme.window.title.text.size = theme.text.size;
			if (!theme.button.margin)
				theme.button.margin = theme.margin;
			if (!theme.button.text.font)
				theme.button.text.font = theme.text.font;
			if (!theme.button.text.size)
				theme.button.text.size = theme.text.size;
			if (!theme.edit.margin)
				theme.edit.margin = theme.margin;
			if (!theme.edit.text.font)
				theme.edit.text.font = theme.text.font;
			if (!theme.edit.text.size)
				theme.edit.text.size = theme.text.size;
			if (!theme.list.margin)
				theme.list.margin = theme.margin;
			if (!theme.list.text.font)
				theme.list.text.font = theme.text.font;
			if (!theme.list.text.size)
				theme.list.text.size = theme.text.size;
		}

		void PostLoadTheme(Theme &theme)
		{
			SetDefaults(theme);
		}

		REGISTER_TYPE(Theme, "theme", PostLoadTheme)
	}
}
