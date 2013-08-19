#include "PositionOrientation.hpp"

namespace page
{
	namespace phys
	{
		namespace attrib
		{
			// construct
			PositionOrientation::PositionOrientation(const math::Vec3 &position, const math::Quat<> &orientation) :
				Position(position), Orientation(orientation) {}
			PositionOrientation::PositionOrientation(const math::Vec3 &position, const math::Vec3 &normal) :
				Position(position), Orientation(normal) {}
			PositionOrientation::PositionOrientation(const math::Matrix<3, 4> &matrix) :
				Position(matrix), Orientation(math::Mat3(matrix)) {}

			// matrix access
			math::Matrix<3, 4> PositionOrientation::GetMatrix() const
			{
				return Position::GetMatrix() * math::Matrix<3, 4>(Orientation::GetMatrix());
			}
			math::Matrix<3, 4> PositionOrientation::GetInvMatrix() const
			{
				return math::Matrix<3, 4>(Orientation::GetInvMatrix()) * Position::GetInvMatrix();
			}
			void PositionOrientation::SetMatrix(const math::Matrix<3, 4> &matrix)
			{
				Position::SetMatrix(matrix);
				Orientation::SetMatrix(math::Mat3(matrix));
			}

			// frame serialization
			Frame PositionOrientation::GetFrame() const
			{
				return
					Position::GetFrame() +
					Orientation::GetFrame();
			}
			void PositionOrientation::Update(const Frame &frame)
			{
				Position::Update(frame);
				Orientation::Update(frame);
			}

			// transform modifiers
			void PositionOrientation::BakeTransform()
			{
				Position::BakeTransform();
				Orientation::BakeTransform();
			}
			void PositionOrientation::UpdateForce()
			{
				Position::UpdateForce();
				Orientation::UpdateForce();
			}
			void PositionOrientation::UpdateDelta()
			{
				Position::UpdateDelta();
				Orientation::UpdateDelta();
			}
		}
	}
}
