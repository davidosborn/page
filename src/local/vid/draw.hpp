#ifndef    page_local_vid_draw_hpp
#   define page_local_vid_draw_hpp

namespace page
{
	namespace phys
	{
		class Camera;
		class Scene;
	}
	namespace gui { class Root; }

	namespace vid
	{
		class DrawContext;

		// interface rendering
		void Draw(DrawContext &, const gui::Root &);

		// scene rendering
		void Draw(DrawContext &, const phys::Scene &);
		void Draw(DrawContext &, const phys::Scene &, const phys::Camera &);
	}
}

#endif
