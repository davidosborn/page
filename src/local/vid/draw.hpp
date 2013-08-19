#ifndef    page_local_vid_draw_hpp
#   define page_local_vid_draw_hpp

namespace page
{
	namespace phys
	{
		class Camera;
		class Scene;
	}
	namespace ui { class UserInterface; }

	namespace vid
	{
		class DrawContext;

		// interface rendering
		void Draw(DrawContext &, const ui::UserInterface &);

		// scene rendering
		void Draw(DrawContext &, const phys::Scene &);
		void Draw(DrawContext &, const phys::Scene &, const phys::Camera &);
	}
}

#endif
