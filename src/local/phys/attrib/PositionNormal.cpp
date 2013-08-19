#include "PositionNormal.hpp"

namespace page
{
	namespace phys
	{
		namespace attrib
		{
			// construct
			PositionNormal::PositionNormal(const math::Vec3 &position, const math::Vec3 &normal) :
				Position(position), Normal(normal) {}
			PositionNormal::PositionNormal(const math::Vec3 &position, const math::Quat<> &orientation) :
				Position(position), Normal(orientation) {}
			PositionNormal::PositionNormal(const math::Matrix<3, 4> &matrix) :
				Position(matrix), Normal(math::Mat3(matrix)) {}

			// matrix access
			math::Matrix<3, 4> PositionNormal::GetMatrix() const
			{
				return Position::GetMatrix() * math::Matrix<3, 4>(Normal::GetMatrix());
			}
			math::Matrix<3, 4> PositionNormal::GetInvMatrix() const
			{
				return math::Matrix<3, 4>(Normal::GetInvMatrix()) * Position::GetInvMatrix();
			}
			void PositionNormal::SetMatrix(const math::Matrix<3, 4> &matrix)
			{
				Position::SetMatrix(matrix);
				Normal::SetMatrix(math::Mat3(matrix));
			}

			// frame serialization
			Frame PositionNormal::GetFrame() const
			{
				return
					Position::GetFrame() +
					Normal::GetFrame();
			}
			void PositionNormal::Update(const Frame &frame)
			{
				Position::Update(frame);
				Normal::Update(frame);
			}

			// transform modifiers
			void PositionNormal::BakeTransform()
			{
				Position::BakeTransform();
				Normal::BakeTransform();
			}
			void PositionNormal::UpdateForce()
			{
				Position::UpdateForce();
				Normal::UpdateForce();
			}
			void PositionNormal::UpdateDelta()
			{
				Position::UpdateDelta();
				Normal::UpdateDelta();
			}
		}
	}
}
