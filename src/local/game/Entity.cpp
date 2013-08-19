#include "../phys/controller/FrameController.hpp"
#include "Entity.hpp"

namespace page { namespace game
{
	// construct
	Entity::Entity(const std::string &name, const cache::Proxy<res::Model> &model) :
		phys::Body(model), name(name)
	{
		// initialize attributes
		SetRadius(.375);
		SetType(phys::Collidable::passive);
		// initialize controllers
		InsertController(phys::FrameController(GetFrame()));
	}

	// attributes
	const std::string &Entity::GetName() const
	{
		return name;
	}
}}
