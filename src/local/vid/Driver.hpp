/**
 * @copyright
 *
 * Copyright (c) 2006-2014 David Osborn
 *
 * Permission is granted to use and redistribute this software in source and
 * binary form, with or without modification, subject to the following
 * conditions:
 *
 * 1. Redistributions in source form must retain the above copyright notice,
 *    this list of conditions, and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions, and the following disclaimer in the same place
 *    and form as other copyright, license, and disclaimer information.
 *
 * 3. Redistributions in binary form must also include an acknowledgement in the
 *    same place and form as other acknowledgements (such as the credits),
 *    similar in substance to the following:
 *
 *       Portions of this software are based on the work of David Osborn.
 *
 * This software is provided "as is", without any express or implied warranty.
 * In no event will the authors be liable for any damages arising out of the use
 * of this software.
 */

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
