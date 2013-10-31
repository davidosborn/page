#include "Collidable.hpp"

namespace page { namespace phys
{
	/*-------------+
	| constructors |
	+-------------*/

	Collidable::Collidable(Type type, float radius) :
		type(type), radius(radius) {}

	/*----------+
	| accessors |
	+----------*/

	Collidable::Type Collidable::GetType() const
	{
		return type;
	}

	void Collidable::SetType(Type type)
	{
		this->type = type;
	}

	float Collidable::GetRadius() const
	{
		return radius;
	}

	void Collidable::SetRadius(float radius)
	{
		this->radius = radius;
	}
}}
