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

#include "PositionNormal.hpp"

namespace page
{
	namespace phys
	{
		namespace attrib
		{
			// construct
			PositionNormal::PositionNormal(const math::Vector<3> &position, const math::Vector<3> &normal) :
				Position(position), Normal(normal) {}
			PositionNormal::PositionNormal(const math::Vector<3> &position, const math::Quat<> &orientation) :
				Position(position), Normal(orientation) {}
			PositionNormal::PositionNormal(const math::Matrix<3, 4> &matrix) :
				Position(matrix), Normal(math::Matrix<3>(matrix)) {}

			// matrix access
			math::Matrix<3, 4> PositionNormal::GetMatrix() const
			{
				return Position::GetMatrix() * math::Matrix<3, 4>(Normal::GetMatrix());
			}
			math::Matrix<3, 4> PositionNormal::GetInvMatrix() const
			{
				return math::Matrix<3, 4>(Normal::GetInvMatrix()) * Position::GetInvMatrix();
			}
			void PositionNormal::SetMatrix(const math::Matrix<3, 4> &matrix)
			{
				Position::SetMatrix(matrix);
				Normal::SetMatrix(math::Matrix<3>(matrix));
			}

			// frame serialization
			Frame PositionNormal::GetFrame() const
			{
				return
					Position::GetFrame()
					Normal::GetFrame();
			}
			void PositionNormal::Update(const Frame &frame)
			{
				Position::Update(frame);
				Normal::Update(frame);
			}

			// transform modifiers
			void PositionNormal::BakeTransform()
			{
				Position::BakeTransform();
				Normal::BakeTransform();
			}
			void PositionNormal::UpdateForce()
			{
				Position::UpdateForce();
				Normal::UpdateForce();
			}
			void PositionNormal::UpdateDelta()
			{
				Position::UpdateDelta();
				Normal::UpdateDelta();
			}
		}
	}
}
