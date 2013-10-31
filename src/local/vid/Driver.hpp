#ifndef    page_local_vid_Driver_hpp
#   define page_local_vid_Driver_hpp

#	include <memory> // unique_ptr

#	include "../math/Aabb.hpp"
#	include "../res/type/Image.hpp"
#	include "../util/class/special_member_functions.hpp" // Polymorphic, Uncopyable

namespace page
{
	namespace gui { class Root; }
	namespace wnd { class Window; }
}

namespace page { namespace vid
{
	class DrawContext;

	/**
	 * The base class for video drivers.
	 *
	 * @note Uses the "Non-Virtual Interface" pattern.
	 */
	class Driver :
		public util::Polymorphic<Driver>,
		public util::Uncopyable<Driver>
	{
		/*-------------+
		| constructors |
		+-------------*/

		public:
		explicit Driver(wnd::Window &);

		/*----------+
		| rendering |
		+----------*/

		/**
		 * Render to the parent window.
		 */
		void Render(const gui::Root &, const math::Aabb<2> & = math::UnitAabb<2>());

		/**
		 * Render to an off-screen image resource.
		 */
		res::Image RenderImage(const math::Vec2u &size);

		/*-----------------+
		| color correction |
		+-----------------*/

		float GetSceneSaturdation() const;
		void SetSceneSaturation(float);

		float GetBrightness() const;
		void SetBrightness(float);

		float GetContrast() const;
		void SetContrast(float);

		/*--------------+
		| window access |
		+--------------*/

		wnd::Window &GetWindow();
		const wnd::Window &GetWindow() const;

		/*---------------+
		| implementation |
		+---------------*/

		private:
		virtual res::Image DoRenderImage(const math::Vec2u &size) = 0;

		/**
		 * Flushes the rendering to the window.  Called at the end of Render().
		 */
		virtual void Flush() = 0;

		/**
		 * Returns a new instance of DrawContext.
		 */
		virtual std::unique_ptr<DrawContext> MakeDrawContext(const math::Aabb<2> &) = 0;

		/*-------------+
		| data members |
		+-------------*/

		private:
		/**
		 * The parent window.
		 */
		wnd::Window &window;

		/**
		 * The scene saturation for rendering.
		 */
		float sceneSaturation = 1;

		/**
		 * The additive brightness for rendering.
		 */
		float brightness = 0;

		/**
		 * The contrast for rendering.
		 */
		float contrast = 1;
	};

	// factory function
	//Driver *MakeDriver(wnd::Window &);
}}

#endif
