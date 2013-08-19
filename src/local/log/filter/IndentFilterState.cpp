#include "../../err/Exception.hpp"
#include "IndentFilterState.hpp"

namespace page
{
	namespace log
	{
		/*----------+
		| observers |
		+----------*/

		unsigned IndentFilterState::GetLevel() const
		{
			return level;
		}

		/*----------+
		| modifiers |
		+----------*/

		void IndentFilterState::Indent()
		{
			++level;
		}

		void IndentFilterState::Dedent()
		{
			if (!level)
				THROW((err::Exception<err::LogModuleTag, err::RangeTag>("indentation level out of range")))
			--level;
		}
	}
}
