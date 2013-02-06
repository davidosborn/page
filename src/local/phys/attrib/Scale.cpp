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

#include "Scale.hpp"

namespace page
{
	namespace phys
	{
		namespace attrib
		{
			// construct
			Scale::Scale(const math::Vector<3> &scale) :
				scale(scale), lastScale(scale), force(1), delta(1) {}
			Scale::Scale(const math::Matrix<3> &matrix) :
				scale(math::GetScale(matrix)), lastScale(scale),
				force(1), delta(1) {}

			// access
			const math::Vector<3> &Scale::GetScale() const
			{
				return scale;
			}
			void Scale::SetScale(const math::Vector<3> &scale)
			{
				// NOTE: comparing value to avoid redundant dirty marking
				if (Any(scale != this->scale))
				{
					this->scale = scale;
					dirtyTransformSig();
				}
			}

			// matrix access
			math::Matrix<3> Scale::GetMatrix() const
			{
				return ScaleMatrix(scale);
			}
			math::Matrix<3> Scale::GetInvMatrix() const
			{
				return ScaleMatrix(1 / scale);
			}
			void Scale::SetMatrix(const math::Matrix<3> &matrix)
			{
				SetScale(math::GetScale(matrix));
			}

			// transform state
			const math::Vector<3> &Scale::GetLastScale() const
			{
				return lastScale;
			}
			const math::Vector<3> &Scale::GetScaleForce() const
			{
				return force;
			}
			const math::Vector<3> &Scale::GetDeltaScale() const
			{
				return delta;
			}

			// frame serialization
			Frame Scale::GetFrame() const
			{
				Frame frame;
				frame.scale = scale;
				return frame;
			}
			void Scale::Update(const Frame &frame)
			{
				if (frame.scale) SetScale(*frame.scale);
			}

			// transform modifiers
			void Scale::BakeTransform()
			{
				lastScale = scale;
			}
			void Scale::UpdateForce()
			{
				force = scale / lastScale;
			}
			void Scale::UpdateDelta()
			{
				delta = scale / lastScale;
			}
		}
	}
}
