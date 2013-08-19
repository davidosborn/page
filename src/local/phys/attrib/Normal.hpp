#ifndef    page_local_phys_attrib_Normal_hpp
#   define page_local_phys_attrib_Normal_hpp

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
			struct Normal : virtual Transformable
			{
				// construct
				explicit Normal(const math::Vec3 & = math::NormVector<3>());
				explicit Normal(const math::Quat<> &);
				explicit Normal(const math::Mat3 &);

				// access
				const math::Vec3 &GetNormal() const;
				void SetNormal(const math::Vec3 &);

				// orientation access
				math::Quat<> GetOrientation() const;
				void SetOrientation(const math::Quat<> &);

				// matrix access
				math::Mat3 GetMatrix() const;
				math::Mat3 GetInvMatrix() const;
				void SetMatrix(const math::Mat3 &);

				// transform state
				const math::Vec3 &GetLastNormal() const;
				math::Quat<> GetLastOrientation() const;
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
				math::Vec3 normal, lastNormal;
				math::Quat<> torque, spin;
			};
		}
	}
}

#endif
