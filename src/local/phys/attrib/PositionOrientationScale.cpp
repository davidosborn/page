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

#include "PositionOrientationScale.hpp"

namespace page { namespace phys { namespace attrib
{
	/*-------------+
	| constructors |
	+-------------*/

	PositionOrientationScale::PositionOrientationScale(const math::Vec3 &position, const math::Quat<> &orientation, const math::Vec3 &scale) :
		PositionOrientation(position, orientation), Scale(scale) {}

	PositionOrientationScale::PositionOrientationScale(const math::Vec3 &position, const math::Vec3 &normal, const math::Vec3 &scale) :
		PositionOrientation(position, normal), Scale(scale) {}

	PositionOrientationScale::PositionOrientationScale(const math::Mat34 &matrix) :
		PositionOrientation(matrix), Scale(math::Mat3(matrix)) {}

	/*----------+
	| accessors |
	+----------*/

	math::Mat34 PositionOrientationScale::GetMatrix() const
	{
		return PositionOrientation::GetMatrix() * math::Mat34(Scale::GetMatrix());
	}

	math::Mat34 PositionOrientationScale::GetInvMatrix() const
	{
		return math::Mat34(Scale::GetInvMatrix()) * PositionOrientation::GetInvMatrix();
	}

	void PositionOrientationScale::SetMatrix(const math::Mat34 &matrix)
	{
		PositionOrientation::SetMatrix(matrix);
		Scale              ::SetMatrix(math::Mat3(matrix));
	}

	void PositionOrientationScale::SetMatrix(const math::Mat3 &matrix)
	{
		Orientation::SetMatrix(matrix);
		Scale      ::SetMatrix(matrix);
	}

	/*--------------------+
	| frame serialization |
	+--------------------*/

	Frame PositionOrientationScale::GetFrame() const
	{
		return
			PositionOrientation::GetFrame() +
			Scale              ::GetFrame();
	}

	void PositionOrientationScale::SetFrame(const Frame &frame)
	{
		PositionOrientation::SetFrame(frame);
		Scale              ::SetFrame(frame);
	}

	/*-----------------------------+
	| Transformable implementation |
	+-----------------------------*/

	void PositionOrientationScale::BakeTransform()
	{
		PositionOrientation::BakeTransform();
		Scale              ::BakeTransform();
	}

	void PositionOrientationScale::UpdateForce()
	{
		PositionOrientation::UpdateForce();
		Scale              ::UpdateForce();
	}

	void PositionOrientationScale::UpdateDelta()
	{
		PositionOrientation::UpdateDelta();
		Scale              ::UpdateDelta();
	}
}}}
