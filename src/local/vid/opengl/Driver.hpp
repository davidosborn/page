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

#ifndef    page_vid_opengl_Driver_hpp
#   define page_vid_opengl_Driver_hpp

#	include <memory> // unique_ptr
#	include "../../util/Signal.hpp" // ScopedConnection
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
				explicit Driver(env::Window &);

				// off-screen rendering
				res::Image RenderImage(const math::Vector<2, unsigned> &size);

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
				void OnSize(const math::Vector<2, unsigned> &);

				util::ScopedConnection focusCon, sizeCon;
				std::unique_ptr<Resources> res;
			};
		}
	}
}

#endif
