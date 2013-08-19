#ifndef    page_local_phys_attrib_PositionNormalScale_hpp
#   define page_local_phys_attrib_PositionNormalScale_hpp

#	include "PositionNormal.hpp"
#	include "Scale.hpp"

namespace page
{
	namespace phys
	{
		namespace attrib
		{
			struct PositionNormalScale : PositionNormal, Scale
			{
				// construct
				explicit PositionNormalScale(
					const math::Vec3 & = 0,
					const math::Vec3 & = math::NormVector<3>(),
					const math::Vec3 & = 1);
				PositionNormalScale(
					const math::Vec3 &,
					const math::Quat<> &,
					const math::Vec3 & = 1);
				explicit PositionNormalScale(const math::Matrix<3, 4> &);

				// matrix access
				math::Matrix<3, 4> GetMatrix() const;
				math::Matrix<3, 4> GetInvMatrix() const;
				void SetMatrix(const math::Matrix<3, 4> &);
				void SetMatrix(const math::Mat3 &);

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
