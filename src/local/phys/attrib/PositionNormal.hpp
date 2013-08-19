#ifndef    page_local_phys_attrib_PositionNormal_hpp
#   define page_local_phys_attrib_PositionNormal_hpp

#	include "Normal.hpp"
#	include "Position.hpp"

namespace page
{
	namespace phys
	{
		namespace attrib
		{
			struct PositionNormal : Position, Normal
			{
				// construct
				explicit PositionNormal(
					const math::Vec3 & = 0,
					const math::Vec3 & = math::NormVector<3>());
				PositionNormal(
					const math::Vec3 &,
					const math::Quat<> &);
				explicit PositionNormal(const math::Matrix<3, 4> &);

				// matrix access
				math::Matrix<3, 4> GetMatrix() const;
				math::Matrix<3, 4> GetInvMatrix() const;
				void SetMatrix(const math::Matrix<3, 4> &);

				protected:
				// frame serialization
				Frame GetFrame() const;
				void Update(const Frame &);

				// transform modifiers
				void BakeTransform();
				void UpdateForce();
				void UpdateDelta();
			};
		}
	}
}

#endif
