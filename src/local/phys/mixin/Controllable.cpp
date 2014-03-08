/**
 * @copyright
 *
 * Copyright (c) 2006-2014 David Osborn
 *
 * Permission is granted to use and redistribute this software in source and
 * binary form, with or without modification, subject to the following
 * conditions:
 *
 * 1. Redistributions in source form must retain the above copyright notice,
 *    this list of conditions, and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions, and the following disclaimer in the same place
 *    and form as other copyright, license, and disclaimer information.
 *
 * 3. Redistributions in binary form must also include an acknowledgement in the
 *    same place and form as other acknowledgements (such as the credits),
 *    similar in substance to the following:
 *
 *       Portions of this software are based on the work of David Osborn.
 *
 * This software is provided "as is", without any express or implied warranty.
 * In no event will the authors be liable for any damages arising out of the use
 * of this software.
 */

#include <algorithm> // max
#include <cassert>

#include "Controllable.hpp"
#include "../controller/Controller.hpp"

namespace page { namespace phys
{
	/*---------------------+
	| controller execution |
	+---------------------*/

	void Controllable::ApplyControllers(AnimationLayer layer, float deltaTime)
	{
		auto layerIndex(static_cast<unsigned>(layer));
		if (layerIndex >= layers.size()) return;
		auto &controllers(layers[layerIndex]);
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
			Blend(accumFrame, controllerFrame, controller.GetOpacity());
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
		DetachController(controller.lock());
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
