#ifndef    page_local_ui_DrawContext_hpp
#   define page_local_ui_DrawContext_hpp

#	include "../res/type/Font.hpp" // TextAlign
#	include "../res/type/Theme.hpp"

namespace page { namespace vid { class DrawContext; }}

namespace page { namespace gui
{
	struct DrawContext
	{
		typedef vid::DrawContext Base;

		// construct
		DrawContext(Base &, const res::Theme &);

		// base context access
		Base &GetBase();
		const Base &GetBase() const;

		// theme access
		const res::Theme &GetTheme() const;

		// metrics
		math::Vec2 GetScale() const;

		// image rendering
		void DrawBackground(
			const res::Theme::Background &,
			const math::Aabb<2> & = math::Aabb<2>(0, 1));
		void DrawFrame(
			const res::Theme::Frame &,
			const math::Aabb<2> & = math::Aabb<2>(0, 1));
		void DrawDecorations(
			const res::Theme::Decorations &,
			const math::Aabb<2> & = math::Aabb<2>(0, 1),
			bool background = false);
		void DrawImage(
			const cache::Proxy<res::Image> &,
			const math::Vec2 &position = 0,
			const math::Vec2 &size = 0,
			const math::Vector<2, bool> &tile = false,
			const math::Vector<2, bool> &flip = false,
			const math::Vec2 &center = 0, float angle = 0);

		// text rendering
		void DrawText(
			const res::Theme::Text &, const std::string &text,
			const math::Aabb<2> & = math::Aabb<2>(0, 1),
			const math::RgbaColor<> &color = math::WhiteRgbaColor(),
			bool wrap = true, res::TextAlign = res::leftTextAlign);

		private:
		Base &base;
		const res::Theme &theme;
		float scale;
	};
}}

#endif
