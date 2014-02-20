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

#include <cassert>

#include "Normal.hpp"

namespace page { namespace phys { namespace attrib
{
	/*-------------+
	| constructors |
	+-------------*/

	Normal::Normal(const math::Vec3 &value) :
		value(value) {}

	Normal::Normal(const math::Quat<> &orientation) :
		value(orientation * math::NormVector<3>()) {}

	Normal::Normal(const math::Mat3 &matrix) :
		value(GetRotation(matrix) * math::NormVector<3>()) {}

	/*----------+
	| accessors |
	+----------*/

	const math::Vec3 &Normal::GetNormal() const
	{
		return value;
	}

	void Normal::SetNormal(const math::Vec3 &value)
	{
		assert(All(Near(value, Norm(value))));
		// NOTE: comparing value to avoid redundant dirty marking
		if (Any(value != this->value))
		{
			// HACK: normalize to correct floating-point drift
			this->value = Norm(value);
			dirtyTransformSig();
		}
	}

	math::Quat<> Normal::GetOrientation() const
	{
		return math::Quat<>(math::NormVector<3>(), value);
	}

	void Normal::SetOrientation(const math::Quat<> &orientation)
	{
		assert(Near(orientation, Norm(orientation)));
		SetNormal(orientation * math::NormVector<3>());
	}

	math::Mat3 Normal::GetMatrix() const
	{
		return RotationMatrix(math::NormVector<3>(), value);
	}

	math::Mat3 Normal::GetInvMatrix() const
	{
		return Tpos(GetMatrix());
	}

	void Normal::SetMatrix(const math::Mat3 &matrix)
	{
		SetNormal(GetRotation(matrix) * math::NormVector<3>());
	}

	const math::Vec3 &Normal::GetLastNormal() const
	{
		return lastValue;
	}

	math::Quat<> Normal::GetLastOrientation() const
	{
		return math::Quat<>(math::NormVector<3>(), lastValue);
	}

	const math::Quat<> &Normal::GetTorque() const
	{
		return torque;
	}

	const math::Quat<> &Normal::GetSpin() const
	{
		return spin;
	}

	/*--------------------+
	| frame serialization |
	+--------------------*/

	Frame Normal::GetFrame() const
	{
		Frame frame;
		frame.normal = value;
		return frame;
	}

	void Normal::SetFrame(const Frame &frame)
	{
		if (frame.normal) SetNormal(*frame.normal);
		else if (frame.orientation) SetOrientation(*frame.orientation);
	}

	/*-----------------------------+
	| Transformable implementation |
	+-----------------------------*/

	void Normal::BakeTransform()
	{
		lastValue = value;
	}

	void Normal::UpdateForce()
	{
		torque = math::Quat<>(lastValue, value);
	}

	void Normal::UpdateDelta()
	{
		spin = math::Quat<>(lastValue, value);
	}
}}}
