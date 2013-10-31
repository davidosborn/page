#include <cassert>
#include "Orientation.hpp"

namespace page { namespace phys { namespace attrib
{
	/*-------------+
	| constructors |
	+-------------*/

	Orientation::Orientation(const math::Quat<> &value) :
		value(value) {}

	Orientation::Orientation(const math::Vec3 &normal) :
		value(math::NormVector<3>(), normal) {}

	Orientation::Orientation(const math::Mat3 &matrix) :
		value(math::GetOrientation(matrix)) {}

	/*----------+
	| accessors |
	+----------*/

	const math::Quat<> &Orientation::GetOrientation() const
	{
		return value;
	}

	void Orientation::SetOrientation(const math::Quat<> &value)
	{
		assert(Near(value, Norm(value)));
		// NOTE: comparing value to avoid redundant dirty marking
		if (value != this->value)
		{
			// HACK: normalize to correct floating-point drift
			this->value = Norm(value);
			dirtyTransformSig();
		}
	}

	math::Vec3 Orientation::GetNormal() const
	{
		return value * math::NormVector<3>();
	}

	void Orientation::SetNormal(const math::Vec3 &normal)
	{
		assert(All(Near(normal, Norm(normal))));
		SetOrientation(math::Quat<>(math::NormVector<3>(), normal));
	}

	math::Mat3 Orientation::GetMatrix() const
	{
		return RotationMatrix(value);
	}

	math::Mat3 Orientation::GetInvMatrix() const
	{
		return RotationMatrix(Inv(value));
	}

	void Orientation::SetMatrix(const math::Mat3 &matrix)
	{
		SetOrientation(math::GetOrientation(matrix));
	}

	const math::Quat<> &Orientation::GetLastOrientation() const
	{
		return lastValue;
	}

	math::Vec3 Orientation::GetLastNormal() const
	{
		return lastValue * math::NormVector<3>();
	}

	const math::Quat<> &Orientation::GetTorque() const
	{
		return torque;
	}

	const math::Quat<> &Orientation::GetSpin() const
	{
		return spin;
	}

	/*--------------------+
	| frame serialization |
	+--------------------*/

	Frame Orientation::GetFrame() const
	{
		Frame frame;
		frame.orientation = value;
		return frame;
	}

	void Orientation::SetFrame(const Frame &frame)
	{
		if (frame.orientation) SetOrientation(*frame.orientation);
		else if (frame.normal) SetNormal(*frame.normal);
	}

	/*-----------------------------+
	| Transformable implementation |
	+-----------------------------*/

	void Orientation::BakeTransform()
	{
		lastValue = value;
	}

	void Orientation::UpdateForce()
	{
		torque = value / lastValue;
	}

	void Orientation::UpdateDelta()
	{
		spin = value / lastValue;
	}
}}}
