#include <cassert>

#include "Button.hpp"

namespace page { namespace inp
{
	std::string GetName(Button button)
	{
		switch (button)
		{
			case Button::left:   return "left";
			case Button::middle: return "middle";
			case Button::right:  return "right";
		}
		assert(!"invalid mouse button");
	}
}
