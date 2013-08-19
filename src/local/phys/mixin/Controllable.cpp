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
			RemoveController(controller.Lock());
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
