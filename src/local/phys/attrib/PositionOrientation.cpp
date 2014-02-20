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

#include "PositionOrientation.hpp"

namespace page { namespace phys { namespace attrib
{
	/*-------------+
	| constructors |
	+-------------*/

	PositionOrientation::PositionOrientation(const math::Vec3 &position, const math::Quat<> &orientation) :
		Position(position), Orientation(orientation) {}

	PositionOrientation::PositionOrientation(const math::Vec3 &position, const math::Vec3 &normal) :
		Position(position), Orientation(normal) {}

	PositionOrientation::PositionOrientation(const math::Mat34 &matrix) :
		Position(matrix), Orientation(math::Mat3(matrix)) {}

	/*----------+
	| accessors |
	+----------*/

	math::Mat34 PositionOrientation::GetMatrix() const
	{
		return Position::GetMatrix() * math::Mat34(Orientation::GetMatrix());
	}

	math::Mat34 PositionOrientation::GetInvMatrix() const
	{
		return math::Mat34(Orientation::GetInvMatrix()) * Position::GetInvMatrix();
	}

	void PositionOrientation::SetMatrix(const math::Mat34 &matrix)
	{
		Position   ::SetMatrix(matrix);
		Orientation::SetMatrix(math::Mat3(matrix));
	}

	/*--------------------+
	| frame serialization |
	+--------------------*/

	Frame PositionOrientation::GetFrame() const
	{
		return
			Position   ::GetFrame() +
			Orientation::GetFrame();
	}

	void PositionOrientation::SetFrame(const Frame &frame)
	{
		Position   ::SetFrame(frame);
		Orientation::SetFrame(frame);
	}

	/*-----------------------------+
	| Transformable implementation |
	+-----------------------------*/

	void PositionOrientation::BakeTransform()
	{
		Position   ::BakeTransform();
		Orientation::BakeTransform();
	}

	void PositionOrientation::UpdateForce()
	{
		Position   ::UpdateForce();
		Orientation::UpdateForce();
	}

	void PositionOrientation::UpdateDelta()
	{
		Position   ::UpdateDelta();
		Orientation::UpdateDelta();
	}
}}}
