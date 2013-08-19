#ifndef    page_local_phys_mixin_Positionable_hpp
#   define page_local_phys_mixin_Positionable_hpp

#	include "../../math/fwd.hpp" // Vector

namespace page
{
	namespace phys
	{
		struct Positionable
		{
			// destroy
			virtual ~Positionable();

			// access
			virtual const math::Vec3 &GetPosition() const = 0;
			virtual void SetPosition(const math::Vec3 &) = 0;

			// delta state
			virtual const math::Vec3 &GetLastPosition() const = 0;
			// NOTE: could provide velocity, but don't current need it
		};
	}
}

#endif
