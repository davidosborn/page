#ifndef    page_local_phys_mixin_Controllable_hpp
#   define page_local_phys_mixin_Controllable_hpp

#	include <memory> // {shared,weak}_ptr
#	include <vector>

#	include "../Controller.hpp" // Controller::Layer
#	include "../Frame.hpp"

namespace page
{
	namespace phys
	{
		typedef std::weak_ptr<Controller> ControllerProxy;

		struct Controllable
		{
			// destroy
			virtual ~Controllable();

			// controller execution
			void Update(Controller::Layer, float deltaTime);

			// controller modifiers
			ControllerProxy InsertController(const Controller &);
			ControllerProxy InsertController(const std::shared_ptr<Controller> &);
			void RemoveController(const ControllerProxy &);
			void RemoveController(const std::shared_ptr<Controller> &);
			void RemoveControllers();

			protected:
			// frame serialization
			virtual Frame GetFrame() const = 0;
			virtual void Update(const Frame &) = 0;

			private:
			typedef std::vector<std::shared_ptr<Controller>> Controllers;
			typedef std::vector<Controllers> Layers;
			Layers layers;
		};
	}
}

#endif
