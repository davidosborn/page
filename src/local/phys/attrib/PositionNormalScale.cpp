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
