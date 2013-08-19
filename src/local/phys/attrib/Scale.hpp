#ifndef    page_local_phys_attrib_Scale_hpp
#   define page_local_phys_attrib_Scale_hpp

#	include "../../math/Matrix.hpp"
#	include "../../math/Vector.hpp"
#	include "../Frame.hpp"
#	include "../mixin/Transformable.hpp"

namespace page
{
	namespace phys
	{
		namespace attrib
		{
			struct Scale : virtual Transformable
			{
				// construct
				explicit Scale(const math::Vec3 & = 1);
				explicit Scale(const math::Mat3 &);

				// access
				const math::Vec3 &GetScale() const;
				void SetScale(const math::Vec3 &);

				// matrix access
				math::Mat3 GetMatrix() const;
				math::Mat3 GetInvMatrix() const;
				void SetMatrix(const math::Mat3 &);

				// transform state
				const math::Vec3 &GetLastScale() const;
				const math::Vec3 &GetScaleForce() const;
				const math::Vec3 &GetDeltaScale() const;

				protected:
				// frame serialization
				Frame GetFrame() const;
				void Update(const Frame &);

				// transform modifiers
				void BakeTransform();
				void UpdateForce();
				void UpdateDelta();

				private:
				math::Vec3 scale, lastScale, force, delta;
			};
		}
	}
}

#endif
