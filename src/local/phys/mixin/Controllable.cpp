#include <algorithm> // max
#include <cassert>

#include "../Attribute.hpp" // Attribute::{Get,Set}Frame
#include "Controllable.hpp"

namespace page { namespace phys
{
	/*---------------------+
	| controller execution |
	+---------------------*/

	void Controllable::ApplyControllers(AnimationLayer layer, float deltaTime)
	{
		auto layerIndex(static_cast<unsigned>(layer));
		if (layerIndex >= layers.size()) return;
		auto &controllers(layers[layer]);
		if (controllers.empty()) return;

		// execute controllers
		Frame baseFrame(GetFrame()), accumFrame;
		for (Controllers::iterator iter(controllers.begin()); iter != controllers.end();)
		{
			Controller &controller(**iter);
			controller.Update(deltaTime);
			if (!controller.IsAlive())
			{
				iter = controllers.erase(iter);
				continue;
			}
			Frame controllerFrame(controller.GetFrame(baseFrame, accumFrame));
			Blend(accumFrame, controllerFrame, controller.GetOpacity(), baseFrame);
			++iter;
		}
		SetFrame(accumFrame);
	}

	void Controllable::ApplyControllers(float deltaTime)
	{
		for (unsigned i = 0; i < layers.size(); ++i)
			ApplyControllers(static_cast<AnimationLayer>(i), deltaTime);
	}

	/*----------------------+
	| controller attachment |
	+----------------------*/

	std::weak_ptr<Controller> Controllable::AttachController(const Controller &controller)
	{
		return AttachController(std::shared_ptr<Controller>(controller.Clone()));
	}

	std::weak_ptr<Controller> Controllable::AttachController(const std::shared_ptr<Controller> &controller)
	{
		assert(controller);
		auto layerIndex(static_cast<unsigned>(controller->GetLayer()));
		layers.resize(std::max(layerIndex + 1, layers.size()));
		layers[layerIndex].push_back(controller);
		return std::weak_ptr<Controller>(controller);
	}

	void Controllable::DetachController(const std::weak_ptr<Controller> &controller)
	{
		RemoveController(controller.lock());
	}

	void Controllable::DetachController(const std::shared_ptr<Controller> &controller)
	{
		assert(controller);
		controller->Kill();
	}

	void Controllable::DetachAllControllers()
	{
		for (const auto &controllers : layers)
			for (const auto &controller : controllers)
				controller->Kill();
	}
}}
