#ifndef    page_local_log_filter_IndentFilterState_hpp
#   define page_local_log_filter_IndentFilterState_hpp

#	include "../../util/class/Monostate.hpp"

namespace page
{
	namespace log
	{
		class IndentFilterState :
			public util::Monostate<IndentFilterState>
		{
			/*----------+
			| observers |
			+----------*/

			public:
			unsigned GetLevel() const;

			/*----------+
			| modifiers |
			+----------*/

			public:
			void Indent();
			void Dedent();

			/*-------------+
			| data members |
			+-------------*/

			private:
			unsigned level = 0;
		};
	}
}

#endif
