#include <cassert>

#include "Button.hpp"

namespace page { namespace inp
{
	std::string GetName(Button button)
	{
		switch (button)
		{
			case leftButton:   return "left";
			case middleButton: return "middle";
			case rightButton:  return "right";
		}
		assert(!"invalid mouse button");
	}
}
