#include "../phys/controller/AnimationController.hpp"
#include "../res/type/Object.hpp"
#include "Object.hpp"

namespace page { namespace game
{
	// construct
	Object::Object(const res::Object &object) :
		Entity(object.name, object.model)
	{
		// initialize attributes
		if (object.radius) SetRadius(object.radius);
		SetScale(GetScale() * object.scale);
		// initialize controllers
		if (object.animation.ambient)
			InsertController(phys::AnimationController(*object.animation.ambient));
	}
}}
