#include <cassert>
#include "Orientation.hpp"

namespace page
{
	namespace phys
	{
		namespace attrib
		{
			// construct
			Orientation::Orientation(const math::Quat<> &orientation) :
				orientation(orientation), lastOrientation(orientation) {}
			Orientation::Orientation(const math::Vec3 &normal) :
				orientation(math::NormVector<3>(), normal),
				lastOrientation(orientation) {}
			Orientation::Orientation(const math::Mat3 &matrix) :
				orientation(math::GetOrientation(matrix)),
				lastOrientation(orientation) {}

			// access
			const math::Quat<> &Orientation::GetOrientation() const
			{
				return orientation;
			}
			void Orientation::SetOrientation(const math::Quat<> &orientation)
			{
				assert(Near(orientation, Norm(orientation)));
				// NOTE: comparing value to avoid redundant dirty marking
				if (orientation != this->orientation)
				{
					// HACK: normalize to correct floating-point drift
					this->orientation = Norm(orientation);
					dirtyTransformSig();
				}
			}

			// normal access
			math::Vec3 Orientation::GetNormal() const
			{
				return orientation * math::NormVector<3>();
			}
			void Orientation::SetNormal(const math::Vec3 &normal)
			{
				assert(All(Near(normal, Norm(normal))));
				SetOrientation(math::Quat<>(math::NormVector<3>(), normal));
			}

			// matrix access
			math::Mat3 Orientation::GetMatrix() const
			{
				return RotationMatrix(orientation);
			}
			math::Mat3 Orientation::GetInvMatrix() const
			{
				return RotationMatrix(Inv(orientation));
			}
			void Orientation::SetMatrix(const math::Mat3 &matrix)
			{
				SetOrientation(math::GetOrientation(matrix));
			}

			// transform state
			const math::Quat<> &Orientation::GetLastOrientation() const
			{
				return lastOrientation;
			}
			math::Vec3 Orientation::GetLastNormal() const
			{
				return lastOrientation * math::NormVector<3>();
			}
			const math::Quat<> &Orientation::GetTorque() const
			{
				return torque;
			}
			const math::Quat<> &Orientation::GetSpin() const
			{
				return spin;
			}

			// frame serialization
			Frame Orientation::GetFrame() const
			{
				Frame frame;
				frame.orientation = orientation;
				return frame;
			}
			void Orientation::Update(const Frame &frame)
			{
				if (frame.orientation) SetOrientation(*frame.orientation);
				else if (frame.normal) SetNormal(*frame.normal);
			}

			// transform modifiers
			void Orientation::BakeTransform()
			{
				lastOrientation = orientation;
			}
			void Orientation::UpdateForce()
			{
				torque = orientation / lastOrientation;
			}
			void Orientation::UpdateDelta()
			{
				spin = orientation / lastOrientation;
			}
		}
	}
}
