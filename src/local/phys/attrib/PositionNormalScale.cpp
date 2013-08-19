#include "PositionNormalScale.hpp"

namespace page
{
	namespace phys
	{
		namespace attrib
		{
			// construct
			PositionNormalScale::PositionNormalScale(const math::Vec3 &position, const math::Vec3 &normal, const math::Vec3 &scale) :
				PositionNormal(position, normal), Scale(scale) {}
			PositionNormalScale::PositionNormalScale(const math::Vec3 &position, const math::Quat<> &orientation, const math::Vec3 &scale) :
				PositionNormal(position, orientation), Scale(scale) {}
			PositionNormalScale::PositionNormalScale(const math::Matrix<3, 4> &matrix) :
				PositionNormal(matrix), Scale(math::Mat3(matrix)) {}

			// matrix access
			math::Matrix<3, 4> PositionNormalScale::GetMatrix() const
			{
				return PositionNormal::GetMatrix() * math::Matrix<3, 4>(Scale::GetMatrix());
			}
			math::Matrix<3, 4> PositionNormalScale::GetInvMatrix() const
			{
				return math::Matrix<3, 4>(Scale::GetInvMatrix()) * PositionNormal::GetInvMatrix();
			}
			void PositionNormalScale::SetMatrix(const math::Matrix<3, 4> &matrix)
			{
				PositionNormal::SetMatrix(matrix);
				Scale::SetMatrix(math::Mat3(matrix));
			}
			void PositionNormalScale::SetMatrix(const math::Mat3 &matrix)
			{
				Normal::SetMatrix(matrix);
				Scale::SetMatrix(matrix);
			}

			// frame serialization
			Frame PositionNormalScale::GetFrame() const
			{
				return
					PositionNormal::GetFrame() +
					Scale::GetFrame();
			}
			void PositionNormalScale::Update(const Frame &frame)
			{
				PositionNormal::Update(frame);
				Scale::Update(frame);
			}

			// transform modifiers
			void PositionNormalScale::BakeTransform()
			{
				PositionNormal::BakeTransform();
				Scale::BakeTransform();
			}
			void PositionNormalScale::UpdateForce()
			{
				PositionNormal::UpdateForce();
				Scale::UpdateForce();
			}
			void PositionNormalScale::UpdateDelta()
			{
				PositionNormal::UpdateDelta();
				Scale::UpdateDelta();
			}
		}
	}
}
