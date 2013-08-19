#include <cassert>

#include "Normal.hpp"

namespace page
{
	namespace phys
	{
		namespace attrib
		{
			// construct
			Normal::Normal(const math::Vec3 &normal) :
				normal(normal), lastNormal(normal) {}
			Normal::Normal(const math::Quat<> &orientation) :
				normal(orientation * math::NormVector<3>()),
				lastNormal(normal) {}
			Normal::Normal(const math::Mat3 &matrix) :
				normal(GetRotation(matrix) * math::NormVector<3>()),
				lastNormal(normal) {}

			// access
			const math::Vec3 &Normal::GetNormal() const
			{
				return normal;
			}
			void Normal::SetNormal(const math::Vec3 &normal)
			{
				assert(All(Near(normal, Norm(normal))));
				// NOTE: comparing value to avoid redundant dirty marking
				if (Any(normal != this->normal))
				{
					// HACK: normalize to correct floating-point drift
					this->normal = Norm(normal);
					dirtyTransformSig();
				}
			}

			// orientation access
			math::Quat<> Normal::GetOrientation() const
			{
				return math::Quat<>(math::NormVector<3>(), normal);
			}
			void Normal::SetOrientation(const math::Quat<> &orientation)
			{
				assert(Near(orientation, Norm(orientation)));
				SetNormal(orientation * math::NormVector<3>());
			}

			// matrix access
			math::Mat3 Normal::GetMatrix() const
			{
				return RotationMatrix(math::NormVector<3>(), normal);
			}
			math::Mat3 Normal::GetInvMatrix() const
			{
				return Tpos(GetMatrix());
			}
			void Normal::SetMatrix(const math::Mat3 &matrix)
			{
				SetNormal(GetRotation(matrix) * math::NormVector<3>());
			}

			// transform state
			const math::Vec3 &Normal::GetLastNormal() const
			{
				return lastNormal;
			}
			math::Quat<> Normal::GetLastOrientation() const
			{
				return math::Quat<>(math::NormVector<3>(), lastNormal);
			}
			const math::Quat<> &Normal::GetTorque() const
			{
				return torque;
			}
			const math::Quat<> &Normal::GetSpin() const
			{
				return spin;
			}

			// frame serialization
			Frame Normal::GetFrame() const
			{
				Frame frame;
				frame.normal = normal;
				return frame;
			}
			void Normal::Update(const Frame &frame)
			{
				if (frame.normal) SetNormal(*frame.normal);
				else if (frame.orientation) SetOrientation(*frame.orientation);
			}

			// transform modifiers
			void Normal::BakeTransform()
			{
				lastNormal = normal;
			}
			void Normal::UpdateForce()
			{
				torque = math::Quat<>(lastNormal, normal);
			}
			void Normal::UpdateDelta()
			{
				spin = math::Quat<>(lastNormal, normal);
			}
		}
	}
}
