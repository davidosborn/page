#include "Positionable.hpp"

namespace page
{
	namespace phys
	{
		math::Vec3 Positionable::GetVelocity() const
		{
			return GetPosition() - GetLastPosition();
		}
	}
}
