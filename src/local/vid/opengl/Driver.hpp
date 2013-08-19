#ifndef    page_local_vid_opengl_Driver_hpp
#   define page_local_vid_opengl_Driver_hpp

#	include <memory> // unique_ptr

#	include <boost/signals/connection.hpp> // scoped_connection

#	include "../Driver.hpp"

namespace page
{
	namespace vid
	{
		namespace opengl
		{
			class Resources;

			struct Driver : vid::Driver
			{
				// construct
				explicit Driver(wnd::Window &);

				// off-screen rendering
				res::Image RenderImage(const math::Vec2u &size);

				protected:
				// deferred initialization
				// must be called at end of derived constructor
				void Init();

				// preemptive destruction
				// must be called at beginning of derived destructor
				void Deinit();

				private:
				// adapter extension initialization
				virtual void InitAdapterExt() = 0;

				// viewport modifiers
				void SetViewport();

				// draw context factory function
				vid::DrawContext *MakeDrawContext(const math::Aabb<2> &);

				// signal handlers
				void OnFocus(bool);
				void OnSize(const math::Vec2u &);

				boost::signals::scoped_connection focusCon, sizeCon;
				std::unique_ptr<Resources> res;
			};
		}
	}
}

#endif
