#include "../phys/controller/FrameController.hpp"
#include "Entity.hpp"

namespace page { namespace game
{
	/*-------------+
	| constructors |
	+-------------*/

	Entity::Entity(const std::string &name, const cache::Proxy<res::Model> &model) :
		name(name), body(std::make_shared<phys::Body>(model))
	{
		// initialize attributes
		body->SetRadius(.375);
		body->SetType(phys::Collidable::Type::passive);

		// initialize controllers
		// FIXME: disabled this, not sure what it was doing, locking the frame?
		//body->AttachController(phys::FrameController(body->GetFrame()));
	}

	Entity::Entity(const Entity &other) :
		name(other.name), body(std::make_shared<phys::Body>(other.GetBody())) {}

	/*----------+
	| observers |
	+----------*/

	const std::string &Entity::GetName() const
	{
		return name;
	}

	phys::Body &Entity::GetBody()
	{
		return *body;
	}

	const phys::Body &Entity::GetBody() const
	{
		return *body;
	}

	const std::shared_ptr<phys::Body> &Entity::GetBodyPtr() const
	{
		return body;
	}
}}
