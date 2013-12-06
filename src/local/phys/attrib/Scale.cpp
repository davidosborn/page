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
