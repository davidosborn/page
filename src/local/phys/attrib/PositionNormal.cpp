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

#include "PositionNormal.hpp"

namespace page { namespace phys { namespace attrib
{
	/*-------------+
	| constructors |
	+-------------*/

	PositionNormal::PositionNormal(const math::Vec3 &position, const math::Vec3 &normal) :
		Position(position), Normal(normal) {}

	PositionNormal::PositionNormal(const math::Vec3 &position, const math::Quat<> &orientation) :
		Position(position), Normal(orientation) {}

	PositionNormal::PositionNormal(const math::Mat34 &matrix) :
		Position(matrix), Normal(math::Mat3(matrix)) {}

	/*----------+
	| accessors |
	+----------*/

	math::Mat34 PositionNormal::GetMatrix() const
	{
		return Position::GetMatrix() * math::Mat34(Normal::GetMatrix());
	}

	math::Mat34 PositionNormal::GetInvMatrix() const
	{
		return math::Mat34(Normal::GetInvMatrix()) * Position::GetInvMatrix();
	}

	void PositionNormal::SetMatrix(const math::Mat34 &matrix)
	{
		Position::SetMatrix(matrix);
		Normal  ::SetMatrix(math::Mat3(matrix));
	}

	/*--------------------+
	| frame serialization |
	+--------------------*/

	Frame PositionNormal::GetFrame() const
	{
		return
			Position::GetFrame() +
			Normal  ::GetFrame();
	}

	void PositionNormal::SetFrame(const Frame &frame)
	{
		Position::SetFrame(frame);
		Normal  ::SetFrame(frame);
	}

	/*-----------------------------+
	| Transformable implementation |
	+-----------------------------*/

	void PositionNormal::BakeTransform()
	{
		Position::BakeTransform();
		Normal  ::BakeTransform();
	}

	void PositionNormal::UpdateForce()
	{
		Position::UpdateForce();
		Normal  ::UpdateForce();
	}

	void PositionNormal::UpdateDelta()
	{
		Position::UpdateDelta();
		Normal  ::UpdateDelta();
	}
}}}
