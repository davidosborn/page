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

#include "PositionNormalScale.hpp"

namespace page { namespace phys { namespace attrib
{
	/*-------------+
	| constructors |
	+-------------*/

	PositionNormalScale::PositionNormalScale(const math::Vec3 &position, const math::Vec3 &normal, const math::Vec3 &scale) :
		PositionNormal(position, normal), Scale(scale) {}

	PositionNormalScale::PositionNormalScale(const math::Vec3 &position, const math::Quat<> &orientation, const math::Vec3 &scale) :
		PositionNormal(position, orientation), Scale(scale) {}

	PositionNormalScale::PositionNormalScale(const math::Mat34 &matrix) :
		PositionNormal(matrix), Scale(math::Mat3(matrix)) {}

	/*----------+
	| accessors |
	+----------*/

	math::Mat34 PositionNormalScale::GetMatrix() const
	{
		return PositionNormal::GetMatrix() * math::Mat34(Scale::GetMatrix());
	}

	math::Mat34 PositionNormalScale::GetInvMatrix() const
	{
		return math::Mat34(Scale::GetInvMatrix()) * PositionNormal::GetInvMatrix();
	}

	void PositionNormalScale::SetMatrix(const math::Mat34 &matrix)
	{
		PositionNormal::SetMatrix(matrix);
		Scale         ::SetMatrix(math::Mat3(matrix));
	}

	void PositionNormalScale::SetMatrix(const math::Mat3 &matrix)
	{
		Normal::SetMatrix(matrix);
		Scale ::SetMatrix(matrix);
	}

	/*--------------------+
	| frame serialization |
	+--------------------*/

	Frame PositionNormalScale::GetFrame() const
	{
		return
			PositionNormal::GetFrame() +
			Scale         ::GetFrame();
	}
	void PositionNormalScale::SetFrame(const Frame &frame)
	{
		PositionNormal::SetFrame(frame);
		Scale         ::SetFrame(frame);
	}

	/*-----------------------------+
	| Transformable implementation |
	+-----------------------------*/

	void PositionNormalScale::BakeTransform()
	{
		PositionNormal::BakeTransform();
		Scale         ::BakeTransform();
	}
	void PositionNormalScale::UpdateForce()
	{
		PositionNormal::UpdateForce();
		Scale         ::UpdateForce();
	}
	void PositionNormalScale::UpdateDelta()
	{
		PositionNormal::UpdateDelta();
		Scale         ::UpdateDelta();
	}
}}}
