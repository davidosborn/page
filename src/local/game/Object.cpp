#include "../phys/controller/AnimationController.hpp"
#include "../phys/node/Body.hpp"
#include "../res/type/Object.hpp"
#include "Object.hpp"

namespace page { namespace game
{
	// construct
	Object::Object(const res::Object &object) :
		Entity(object.name, object.model)
	{
		auto &body(GetBody());

		// initialize attributes
		if (object.radius) body.SetRadius(object.radius);
		body.SetScale(body.GetScale() * object.scale);

		// initialize controllers
		if (object.animation.ambient)
			body.AttachController(phys::AnimationController(*object.animation.ambient));
	}
}}
