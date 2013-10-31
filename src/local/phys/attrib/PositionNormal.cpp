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
