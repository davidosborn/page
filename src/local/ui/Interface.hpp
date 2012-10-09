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

#ifndef    page_local_ui_Interface_hpp
#   define page_local_ui_Interface_hpp

#	include "../math/Aabb.hpp"
#	include "WidgetContainer.hpp"

namespace page
{
	namespace res { class Theme; }
	namespace vid { class DrawContext; }

	namespace ui
	{
		class Widget;

		struct Interface : WidgetContainer
		{
			// construct/destroy
			Interface(const std::shared_ptr<const res::Theme> &);
			virtual ~Interface();

			// metrics
			math::Aabb<2> GetBounds(const Widget &, const vid::DrawContext &) const;
			math::Aabb<2> GetBounds(const Widget &, float aspect) const;

			// theme access
			const res::Theme &GetTheme() const;
			void SetTheme(const std::shared_ptr<const res::Theme> &);

			// rendering
			void Draw(vid::DrawContext &) const;

			// update
			void Update(float deltaTime);
			bool UpdateCursor(const math::Vector<2> &position, float aspect);

			private:
			std::shared_ptr<const res::Theme> theme;
		};
	}
}

#endif
