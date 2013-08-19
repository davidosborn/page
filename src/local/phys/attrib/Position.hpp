#ifndef    page_local_phys_attrib_Position_hpp
#   define page_local_phys_attrib_Position_hpp

#	include "../../math/Matrix.hpp"
#	include "../../math/Vector.hpp"
#	include "../Frame.hpp"
#	include "../mixin/Positionable.hpp"
#	include "../mixin/Transformable.hpp"

namespace page
{
	namespace phys
	{
		namespace attrib
		{
			struct Position : virtual Positionable, virtual Transformable
			{
				// construct
				explicit Position(const math::Vec3 & = 0);
				explicit Position(const math::Matrix<3, 4> &);

				// access
				const math::Vec3 &GetPosition() const;
				void SetPosition(const math::Vec3 &);

				// matrix access
				math::Matrix<3, 4> GetMatrix() const;
				math::Matrix<3, 4> GetInvMatrix() const;
				void SetMatrix(const math::Matrix<3, 4> &);

				// transform state
				const math::Vec3 &GetLastPosition() const;
				const math::Vec3 &GetForce() const;
				const math::Vec3 &GetVelocity() const;

				protected:
				// frame serialization
				Frame GetFrame() const;
				void Update(const Frame &);

				// transform modifiers
				void BakeTransform();
				void UpdateForce();
				void UpdateDelta();

				private:
				math::Vec3 position, lastPosition, force, velocity;
			};
		}
	}
}

#endif
