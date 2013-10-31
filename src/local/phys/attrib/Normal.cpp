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
		if (value != this->value)
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
