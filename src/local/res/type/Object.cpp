#include "Object.hpp"
#include "Registry.hpp" // REGISTER_TYPE

namespace page
{
	namespace res
	{
		// construct
		Object::Object() : radius(0), scale(1) {}

		REGISTER_TYPE(Object, "object")
	}
}
