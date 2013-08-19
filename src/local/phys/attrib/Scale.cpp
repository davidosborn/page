#include "Scale.hpp"

namespace page
{
	namespace phys
	{
		namespace attrib
		{
			// construct
			Scale::Scale(const math::Vec3 &scale) :
				scale(scale), lastScale(scale), force(1), delta(1) {}
			Scale::Scale(const math::Mat3 &matrix) :
				scale(math::GetScale(matrix)), lastScale(scale),
				force(1), delta(1) {}

			// access
			const math::Vec3 &Scale::GetScale() const
			{
				return scale;
			}
			void Scale::SetScale(const math::Vec3 &scale)
			{
				// NOTE: comparing value to avoid redundant dirty marking
				if (Any(scale != this->scale))
				{
					this->scale = scale;
					dirtyTransformSig();
				}
			}

			// matrix access
			math::Mat3 Scale::GetMatrix() const
			{
				return ScaleMatrix(scale);
			}
			math::Mat3 Scale::GetInvMatrix() const
			{
				return ScaleMatrix(1 / scale);
			}
			void Scale::SetMatrix(const math::Mat3 &matrix)
			{
				SetScale(math::GetScale(matrix));
			}

			// transform state
			const math::Vec3 &Scale::GetLastScale() const
			{
				return lastScale;
			}
			const math::Vec3 &Scale::GetScaleForce() const
			{
				return force;
			}
			const math::Vec3 &Scale::GetDeltaScale() const
			{
				return delta;
			}

			// frame serialization
			Frame Scale::GetFrame() const
			{
				Frame frame;
				frame.scale = scale;
				return frame;
			}
			void Scale::Update(const Frame &frame)
			{
				if (frame.scale) SetScale(*frame.scale);
			}

			// transform modifiers
			void Scale::BakeTransform()
			{
				lastScale = scale;
			}
			void Scale::UpdateForce()
			{
				force = scale / lastScale;
			}
			void Scale::UpdateDelta()
			{
				delta = scale / lastScale;
			}
		}
	}
}
