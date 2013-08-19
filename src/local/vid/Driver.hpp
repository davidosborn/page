#ifndef    page_local_vid_Driver_hpp
#   define page_local_vid_Driver_hpp

#	include "../math/Aabb.hpp"
#	include "../math/fwd.hpp" // Vector
#	include "../res/type/Image.hpp"
#	include "../util/class/special_member_functions.hpp" // Uncopyable

namespace page
{
	namespace phys { class Scene; }
	namespace ui { class UserInterface; }
	namespace wnd { class Window; }

	namespace vid
	{
		class DrawContext;

		struct Driver : util::Uncopyable<Driver>
		{
			// construct/destroy
			explicit Driver(wnd::Window &wnd);
			virtual ~Driver();

			// update
			void Update();

			// rendering
			void Render(const math::Aabb<2> & = math::Aabb<2>(0, 1));

			// off-screen rendering
			virtual res::Image RenderImage(const math::Vec2u &size) = 0;

			// scene modifiers
			void SetSceneSaturation(float);

			// color correction
			float GetBrightness() const;
			float GetContrast() const;
			void SetBrightness(float);
			void SetContrast(float);

			// window access
			wnd::Window &GetWindow();
			const wnd::Window &GetWindow() const;

			// inspiration modifiers
			void Imbue(const phys::Scene *);
			void Imbue(const ui::UserInterface *);

			private:
			// inspiration access
			const phys::Scene *GetScene() const;
			const ui::UserInterface *GetUserInterface() const;

			// framebuffer modifiers
			virtual void Flush() = 0;

			// draw context factory function
			virtual DrawContext *MakeDrawContext(const math::Aabb<2> &) = 0;

			wnd::Window &wnd;
			const phys::Scene *scene;
			const ui::UserInterface *userInterface;
			float sceneSaturation;
			float brightness, contrast;
		};

		// factory function
		Driver *MakeDriver(wnd::Window &);
	}
}

#endif
