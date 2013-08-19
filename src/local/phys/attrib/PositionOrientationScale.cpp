#include "PositionOrientationScale.hpp"

namespace page
{
	namespace phys
	{
		namespace attrib
		{
			// construct
			PositionOrientationScale::PositionOrientationScale(const math::Vec3 &position, const math::Quat<> &orientation, const math::Vec3 &scale) :
				PositionOrientation(position, orientation), Scale(scale) {}
			PositionOrientationScale::PositionOrientationScale(const math::Vec3 &position, const math::Vec3 &normal, const math::Vec3 &scale) :
				PositionOrientation(position, normal), Scale(scale) {}
			PositionOrientationScale::PositionOrientationScale(const math::Matrix<3, 4> &matrix) :
				PositionOrientation(matrix), Scale(math::Mat3(matrix)) {}

			// matrix access
			math::Matrix<3, 4> PositionOrientationScale::GetMatrix() const
			{
				return PositionOrientation::GetMatrix() * math::Matrix<3, 4>(Scale::GetMatrix());
			}
			math::Matrix<3, 4> PositionOrientationScale::GetInvMatrix() const
			{
				return math::Matrix<3, 4>(Scale::GetInvMatrix()) * PositionOrientation::GetInvMatrix();
			}
			void PositionOrientationScale::SetMatrix(const math::Matrix<3, 4> &matrix)
			{
				PositionOrientation::SetMatrix(matrix);
				Scale::SetMatrix(math::Mat3(matrix));
			}
			void PositionOrientationScale::SetMatrix(const math::Mat3 &matrix)
			{
				Orientation::SetMatrix(matrix);
				Scale::SetMatrix(matrix);
			}

			// frame serialization
			Frame PositionOrientationScale::GetFrame() const
			{
				return
					PositionOrientation::GetFrame() +
					Scale::GetFrame();
			}
			void PositionOrientationScale::Update(const Frame &frame)
			{
				PositionOrientation::Update(frame);
				Scale::Update(frame);
			}

			// transform modifiers
			void PositionOrientationScale::BakeTransform()
			{
				PositionOrientation::BakeTransform();
				Scale::BakeTransform();
			}
			void PositionOrientationScale::UpdateForce()
			{
				PositionOrientation::UpdateForce();
				Scale::UpdateForce();
			}
			void PositionOrientationScale::UpdateDelta()
			{
				PositionOrientation::UpdateDelta();
				Scale::UpdateDelta();
			}
		}
	}
}
