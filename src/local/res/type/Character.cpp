#include "Character.hpp"
#include "Registry.hpp" // REGISTER_TYPE

namespace page
{
	namespace res
	{
		// construct
		Character::Character() : radius(0), scale(1) {}

		REGISTER_TYPE(Character, "character")
	}
}
