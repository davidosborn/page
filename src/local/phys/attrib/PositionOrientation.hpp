#ifndef    page_local_phys_attrib_PositionOrientation_hpp
#   define page_local_phys_attrib_PositionOrientation_hpp

#	include "Orientation.hpp"
#	include "Position.hpp"

namespace page
{
	namespace phys
	{
		namespace attrib
		{
			struct PositionOrientation : Position, Orientation
			{
				// construct
				explicit PositionOrientation(
					const math::Vec3 & = 0,
					const math::Quat<> & = math::Quat<>());
				PositionOrientation(
					const math::Vec3 &,
					const math::Vec3 &);
				explicit PositionOrientation(const math::Matrix<3, 4> &);

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
