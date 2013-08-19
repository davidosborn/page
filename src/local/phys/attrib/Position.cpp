#include "Position.hpp"

namespace page
{
	namespace phys
	{
		namespace attrib
		{
			// construct
			Position::Position(const math::Vec3 &position) :
				position(position), lastPosition(position) {}
			Position::Position(const math::Matrix<3, 4> &matrix) :
				position(GetTranslation(matrix)), lastPosition(position) {}

			// access
			const math::Vec3 &Position::GetPosition() const
			{
				return position;
			}
			void Position::SetPosition(const math::Vec3 &position)
			{
				// NOTE: comparing value to avoid redundant dirty marking
				if (Any(position != this->position))
				{
					this->position = position;
					dirtyTransformSig();
				}
			}

			// matrix access
			math::Matrix<3, 4> Position::GetMatrix() const
			{
				return TranslationMatrix(position);
			}
			math::Matrix<3, 4> Position::GetInvMatrix() const
			{
				return TranslationMatrix(-position);
			}
			void Position::SetMatrix(const math::Matrix<3, 4> &matrix)
			{
				SetPosition(GetTranslation(matrix));
			}

			// transform state
			const math::Vec3 &Position::GetLastPosition() const
			{
				return lastPosition;
			}
			const math::Vec3 &Position::GetForce() const
			{
				return force;
			}
			const math::Vec3 &Position::GetVelocity() const
			{
				return velocity;
			}

			// frame serialization
			Frame Position::GetFrame() const
			{
				Frame frame;
				frame.position = position;
				return frame;
			}
			void Position::Update(const Frame &frame)
			{
				if (frame.position) SetPosition(*frame.position);
			}

			// transform modifiers
			void Position::BakeTransform()
			{
				lastPosition = position;
			}
			void Position::UpdateForce()
			{
				force = position - lastPosition;
			}
			void Position::UpdateDelta()
			{
				velocity = position - lastPosition;
			}
		}
	}
}
