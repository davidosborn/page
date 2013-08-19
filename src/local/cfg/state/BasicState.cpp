#include <algorithm> // find, upper_bound

#include "../var/BasicVar.hpp" // BasicVar::key
#include "BasicState.hpp"

namespace page { namespace cfg
{
	/*------------------+
	| cvar registration |
	+------------------*/

	void BasicState::Register(BasicVar *var)
	{
		vars.insert(
			std::upper_bound(
				vars.begin(),
				vars.end(),
				var),
			var);
	}

	void BasicState::Unregister(BasicVar *var)
	{
		vars.erase(std::find(vars.begin(), vars.end(), var));
	}
}}
