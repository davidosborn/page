#ifndef    page_local_log_Indenter_hpp
#   define page_local_log_Indenter_hpp

#	include <memory> // shared_ptr

#	include "../util/class/special_member_functions.hpp" // Unmovable

namespace page
{
	namespace log
	{
		class IndentFilterState;

		/**
		 * Acts as a scope guard for indentation, providing a single level of
		 * indentation for the duration of the object.
		 */
		class Indenter : public util::Unmovable<Indenter>
		{
			/*--------------------------+
			| constructors & destructor |
			+--------------------------*/

			public:
			Indenter();
			explicit Indenter(const std::shared_ptr<IndentFilterState> &);
			~Indenter();

			/*-------------+
			| data members |
			+-------------*/

			private:
			std::shared_ptr<IndentFilterState> state;
		};
	}
}

#endif
