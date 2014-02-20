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

#include "Scale.hpp"

namespace page { namespace phys { namespace attrib
{
	/*-------------+
	| constructors |
	+-------------*/

	Scale::Scale(const math::Vec3 &value) :
		value(value) {}

	Scale::Scale(const math::Mat3 &matrix) :
		value(math::GetScale(matrix)) {}

	/*----------+
	| accessors |
	+----------*/

	const math::Vec3 &Scale::GetScale() const
	{
		return value;
	}

	void Scale::SetScale(const math::Vec3 &value)
	{
		// NOTE: comparing value to avoid redundant dirty marking
		if (Any(value != this->value))
		{
			this->value = value;
			dirtyTransformSig();
		}
	}

	math::Mat3 Scale::GetMatrix() const
	{
		return ScaleMatrix(value);
	}

	math::Mat3 Scale::GetInvMatrix() const
	{
		return ScaleMatrix(1 / value);
	}

	void Scale::SetMatrix(const math::Mat3 &matrix)
	{
		SetScale(math::GetScale(matrix));
	}

	const math::Vec3 &Scale::GetLastScale() const
	{
		return lastValue;
	}

	const math::Vec3 &Scale::GetScaleForce() const
	{
		return force;
	}

	const math::Vec3 &Scale::GetDeltaScale() const
	{
		return delta;
	}

	/*--------------------+
	| frame serialization |
	+--------------------*/

	Frame Scale::GetFrame() const
	{
		Frame frame;
		frame.scale = value;
		return frame;
	}

	void Scale::SetFrame(const Frame &frame)
	{
		if (frame.scale)
			SetScale(*frame.scale);
	}

	/*-----------------------------+
	| Transformable implementation |
	+-----------------------------*/

	void Scale::BakeTransform()
	{
		lastValue = value;
	}

	void Scale::UpdateForce()
	{
		force = value / lastValue;
	}

	void Scale::UpdateDelta()
	{
		delta = value / lastValue;
	}
}}}
