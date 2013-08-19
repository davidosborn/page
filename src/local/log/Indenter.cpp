#include "../util/memory/Deleter.hpp" // Share
#include "filter/IndentFilterState.hpp"
#include "Indenter.hpp"

namespace page
{
	namespace log
	{
		/*--------------------------+
		| constructors & destructor |
		+--------------------------*/

		Indenter::Indenter() :
			Indenter(util::Share(GLOBAL(IndentFilterState))) {}

		Indenter::Indenter(const std::shared_ptr<IndentFilterState> &state) :
			state(state)
		{
			state->Indent();
		}

		Indenter::~Indenter()
		{
			state->Dedent();
		}
	}
}
