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

#include "PositionOrientationScale.hpp"

namespace page
{
	namespace phys
	{
		namespace attrib
		{
			// construct
			PositionOrientationScale::PositionOrientationScale(const math::Vector<3> &position, const math::Quat<> &orientation, const math::Vector<3> &scale) :
				PositionOrientation(position, orientation), Scale(scale) {}
			PositionOrientationScale::PositionOrientationScale(const math::Vector<3> &position, const math::Vector<3> &normal, const math::Vector<3> &scale) :
				PositionOrientation(position, normal), Scale(scale) {}
			PositionOrientationScale::PositionOrientationScale(const math::Matrix<3, 4> &matrix) :
				PositionOrientation(matrix), Scale(math::Matrix<3>(matrix)) {}

			// matrix access
			math::Matrix<3, 4> PositionOrientationScale::GetMatrix() const
			{
				return PositionOrientation::GetMatrix() * math::Matrix<3, 4>(Scale::GetMatrix());
			}
			math::Matrix<3, 4> PositionOrientationScale::GetInvMatrix() const
			{
				return math::Matrix<3, 4>(Scale::GetInvMatrix()) * PositionOrientation::GetInvMatrix();
			}
			void PositionOrientationScale::SetMatrix(const math::Matrix<3, 4> &matrix)
			{
				PositionOrientation::SetMatrix(matrix);
				Scale::SetMatrix(math::Matrix<3>(matrix));
			}
			void PositionOrientationScale::SetMatrix(const math::Matrix<3> &matrix)
			{
				Orientation::SetMatrix(matrix);
				Scale::SetMatrix(matrix);
			}

			// frame serialization
			Frame PositionOrientationScale::GetFrame() const
			{
				return
					PositionOrientation::GetFrame()
					Scale::GetFrame();
			}
			void PositionOrientationScale::Update(const Frame &frame)
			{
				PositionOrientation::Update(frame);
				Scale::Update(frame);
			}

			// transform modifiers
			void PositionOrientationScale::BakeTransform()
			{
				PositionOrientation::BakeTransform();
				Scale::BakeTransform();
			}
			void PositionOrientationScale::UpdateForce()
			{
				PositionOrientation::UpdateForce();
				Scale::UpdateForce();
			}
			void PositionOrientationScale::UpdateDelta()
			{
				PositionOrientation::UpdateDelta();
				Scale::UpdateDelta();
			}
		}
	}
}
