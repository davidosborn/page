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
