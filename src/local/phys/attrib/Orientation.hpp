#ifndef    page_local_phys_attrib_Orientation_hpp
#   define page_local_phys_attrib_Orientation_hpp

#	include "../../math/Matrix.hpp"
#	include "../../math/Quat.hpp"
#	include "../../math/Vector.hpp"
#	include "../Frame.hpp"
#	include "../mixin/Transformable.hpp"

namespace page
{
	namespace phys
	{
		namespace attrib
		{
			struct Orientation : virtual Transformable
			{
				// construct
				explicit Orientation(const math::Quat<> & = math::Quat<>());
				explicit Orientation(const math::Vec3 &normal);
				explicit Orientation(const math::Mat3 &);

				// access
				const math::Quat<> &GetOrientation() const;
				void SetOrientation(const math::Quat<> &);

				// normal access
				math::Vec3 GetNormal() const;
				void SetNormal(const math::Vec3 &);

				// matrix access
				math::Mat3 GetMatrix() const;
				math::Mat3 GetInvMatrix() const;
				void SetMatrix(const math::Mat3 &);

				// transform state
				const math::Quat<> &GetLastOrientation() const;
				math::Vec3 GetLastNormal() const;
				const math::Quat<> &GetTorque() const;
				const math::Quat<> &GetSpin() const;

				protected:
				// frame serialization
				Frame GetFrame() const;
				void Update(const Frame &);

				// transform modifiers
				void BakeTransform();
				void UpdateForce();
				void UpdateDelta();

				private:
				math::Quat<> orientation, lastOrientation, torque, spin;
			};
		}
	}
}

#endif
