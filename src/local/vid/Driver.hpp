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

#ifndef    page_local_vid_Driver_hpp
#   define page_local_vid_Driver_hpp

#	include "../math/Aabb.hpp"
#	include "../math/fwd.hpp" // Vector
#	include "../res/type/Image.hpp"
#	include "../util/NonCopyable.hpp"

namespace page
{
	namespace env { class Window; }
	namespace phys { class Scene; }
	namespace ui { class Interface; }

	namespace vid
	{
		class DrawContext;

		struct Driver : util::NonCopyable
		{
			// construct/destroy
			explicit Driver(env::Window &wnd);
			virtual ~Driver();

			// update
			void Update();

			// rendering
			void Render(const math::Aabb<2> & = math::Aabb<2>(0, 1));

			// off-screen rendering
			virtual res::Image RenderImage(const math::Vector<2, unsigned> &size) = 0;

			// scene modifiers
			void SetSceneSaturation(float);

			// color correction
			float GetBrightness() const;
			float GetContrast() const;
			void SetBrightness(float);
			void SetContrast(float);

			// window access
			env::Window &GetWindow();
			const env::Window &GetWindow() const;

			// inspiration modifiers
			void Imbue(const phys::Scene *);
			void Imbue(const ui::Interface *);

			private:
			// inspiration access
			const phys::Scene *GetScene() const;
			const ui::Interface *GetInterface() const;

			// framebuffer modifiers
			virtual void Flush() = 0;

			// draw context factory function
			virtual DrawContext *MakeDrawContext(const math::Aabb<2> &) = 0;

			env::Window &wnd;
			const phys::Scene *scene;
			const ui::Interface *interface;
			float sceneSaturation;
			float brightness, contrast;
		};

		// factory function
		Driver *MakeDriver(env::Window &);
	}
}

#endif
