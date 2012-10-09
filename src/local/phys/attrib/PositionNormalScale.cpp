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

#include "PositionNormalScale.hpp"

namespace page
{
	namespace phys
	{
		namespace attrib
		{
			// construct
			PositionNormalScale::PositionNormalScale(const math::Vector<3> &position, const math::Vector<3> &normal, const math::Vector<3> &scale) :
				PositionNormal(position, normal), Scale(scale) {}
			PositionNormalScale::PositionNormalScale(const math::Vector<3> &position, const math::Quat<> &orientation, const math::Vector<3> &scale) :
				PositionNormal(position, orientation), Scale(scale) {}
			PositionNormalScale::PositionNormalScale(const math::Matrix<3, 4> &matrix) :
				PositionNormal(matrix), Scale(math::Matrix<3>(matrix)) {}

			// matrix access
			math::Matrix<3, 4> PositionNormalScale::GetMatrix() const
			{
				return PositionNormal::GetMatrix() * math::Matrix<3, 4>(Scale::GetMatrix());
			}
			math::Matrix<3, 4> PositionNormalScale::GetInvMatrix() const
			{
				return math::Matrix<3, 4>(Scale::GetInvMatrix()) * PositionNormal::GetInvMatrix();
			}
			void PositionNormalScale::SetMatrix(const math::Matrix<3, 4> &matrix)
			{
				PositionNormal::SetMatrix(matrix);
				Scale::SetMatrix(math::Matrix<3>(matrix));
			}
			void PositionNormalScale::SetMatrix(const math::Matrix<3> &matrix)
			{
				Normal::SetMatrix(matrix);
				Scale::SetMatrix(matrix);
			}

			// frame serialization
			Frame PositionNormalScale::GetFrame() const
			{
				return
					PositionNormal::GetFrame() +
					Scale::GetFrame();
			}
			void PositionNormalScale::Update(const Frame &frame)
			{
				PositionNormal::Update(frame);
				Scale::Update(frame);
			}

			// transform modifiers
			void PositionNormalScale::BakeTransform()
			{
				PositionNormal::BakeTransform();
				Scale::BakeTransform();
			}
			void PositionNormalScale::UpdateForce()
			{
				PositionNormal::UpdateForce();
				Scale::UpdateForce();
			}
			void PositionNormalScale::UpdateDelta()
			{
				PositionNormal::UpdateDelta();
				Scale::UpdateDelta();
			}
		}
	}
}
