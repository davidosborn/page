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
 *
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

#ifndef    page_local_ui_Widget_hpp
#   define page_local_ui_Widget_hpp

#	include "../math/Vector.hpp"
#	include "../util/Cloneable.hpp"

namespace page
{
	namespace res { class Theme; }

	namespace ui
	{
		class DrawContext;

		struct WidgetSize
		{
			enum Mode
			{
				shrink, // select smallest fitting size
				grow,   // fit size to match other widgets
				max     // select largest fitting size
			};

			// construct
			WidgetSize(
				const math::Vector<2> &min = 0,
				const math::Vector<2, int> &mode = shrink);

			math::Vector<2> min;
			math::Vector<2, int> mode;
		};

		struct Widget : util::Cloneable<Widget>
		{
			typedef WidgetSize Size;

			// construct
			Widget(
				const math::Vector<2> &position = .5,
				const math::Vector<2> &minSize = 0,
				bool visible = true);

			// attribute state
			const math::Vector<2> &GetPosition() const;
			const math::Vector<2> &GetMinSize() const;

			// visibility state
			bool IsEnabled() const;
			bool IsVisible() const;

			// metrics
			Size GetSize(const res::Theme &) const;

			// rendering
			void Draw(DrawContext &) const;

			// update
			void Update(float deltaTime);

			// attribute modifiers
			void SetPosition(const math::Vector<2> &);
			void SetMinSize(const math::Vector<2> &);

			// visibility modifiers
			void Show(bool fade = true);
			void Hide(bool fade = true);
			void SetVisible(bool, bool fade = true);
			void SetOpacity(float);

			// cursor event notification
			virtual bool OnOver(const math::Vector<2> &position, const res::Theme &, const math::Vector<2> &size);
			virtual bool OnDown(const math::Vector<2> &position, const res::Theme &, const math::Vector<2> &size);
			virtual bool OnClick(const math::Vector<2> &position, const res::Theme &, const math::Vector<2> &size);

			// key event notification
			// FIXME: implement

			protected:
			// glow constants
			static const float
				glowFadeInDuration,
				glowFadeOutDuration,
				glowFadeExponent;

			private:
			// virtual functions
			virtual Size CalcSize(const res::Theme &) const = 0;
			virtual void DoDraw(DrawContext &) const = 0;
			virtual void DoUpdate(float deltaTime);

			math::Vector<2> pos, minSize;
			bool visible;
			float opacity;
		};
	}
}

#endif
