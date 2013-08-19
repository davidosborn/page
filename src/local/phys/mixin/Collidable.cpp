#include "Collidable.hpp"

namespace page
{
	namespace phys
	{
		// construct
		Collidable::Collidable(Type type, float radius) :
			type(type), radius(radius) {}

		// state
		Collidable::Type Collidable::GetType() const
		{
			return type;
		}
		float Collidable::GetRadius() const
		{
			return radius;
		}

		// modifiers
		void Collidable::SetType(Type type)
		{
			this->type = type;
		}
		void Collidable::SetRadius(float radius)
		{
			this->radius = radius;
		}
	}
}
