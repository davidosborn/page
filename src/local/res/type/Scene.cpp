#include "Registry.hpp" // REGISTER_TYPE
#include "Scene.hpp"

namespace page
{
	namespace res
	{
		// form construct
		Scene::Form::Form() : scale(math::ScaleVector<3>()) {}

		REGISTER_TYPE(Scene, "scene", 0)
	}
}
