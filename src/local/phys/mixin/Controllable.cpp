/**
 * @section license
 *
 * Copyright (c) 2006-2012 David Osborn
 *
 * Permission is granted to use and redistribute this software in source and
 * binary form, with or without modification, subject to the following
 * conditions:
 *
 * 1. Redistributions in source form must retain the above copyright notice,
 *    this list of conditions, and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions, and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution, and in the same
 *    place and form as other copyright, license, and disclaimer information.
 *
 * As a special exception, distributions of derivative works in binary form may
 * include an acknowledgement in place of the above copyright notice, this list
 * of conditions, and the following disclaimer in the documentation and/or other
 * materials provided with the distribution, and in the same place and form as
 * other acknowledgements, similar in substance to the following:
 *
 *    Portions of this software are based on the work of David Osborn.
 *
 * This software is provided "as is", without any express or implied warranty.
 * In no event will the authors be liable for any damages arising out of the use
 * of this software.
 */

#include <algorithm> // max
#include <cassert>
#include "Controllable.hpp"

namespace page
{
	namespace phys
	{
		// destroy
		Controllable::~Controllable() {}

		// controller execution
		void Controllable::Update(Controller::Layer layer, float deltaTime)
		{
			// NOTE: assuming layer enum is indexable (contiguous from zero)
			if (layer >= layers.size()) return;
			Controllers &controllers(layers[layer]);
			if (controllers.empty()) return;
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
			Update(accumFrame);
		}

		// controller modifiers
		ControllerProxy Controllable::InsertController(const Controller &controller)
		{
			return InsertController(std::shared_ptr<Controller>(controller.Clone()));
		}
		ControllerProxy Controllable::InsertController(const std::shared_ptr<Controller> &controller)
		{
			assert(controller);
			// NOTE: assuming layer enum is indexable (contiguous from zero)
			unsigned layerIndex = controller->GetLayer();
			layers.resize(std::max(layerIndex + 1, layers.size()));
			layers[layerIndex].push_back(controller);
			return ControllerProxy(controller);
		}
		void Controllable::RemoveController(const ControllerProxy &controller)
		{
			RemoveController(controller.lock());
		}
		void Controllable::RemoveController(const std::shared_ptr<Controller> &controller)
		{
			assert(controller);
			controller->Kill();
		}
		void Controllable::RemoveControllers()
		{
			for (Layers::iterator layer(layers.begin()); layer != layers.end(); ++layer)
			{
				Controllers &controllers(*layer);
				for (Controllers::iterator iter(controllers.begin()); iter != controllers.end(); ++iter)
				{
					Controller &controller(**iter);
					controller.Kill();
				}
			}
		}
	}
}
