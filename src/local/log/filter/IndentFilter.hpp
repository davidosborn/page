#ifndef    page_local_log_filter_IndentFilter_hpp
#   define page_local_log_filter_IndentFilter_hpp

#	include "../../util/memory/Deleter.hpp" // Share
#	include "IndentFilterState.hpp"
#	include "PrefixFilter.hpp"

namespace page
{
	namespace log
	{
		class IndentFilter final : public PrefixFilter
		{
			/*--------------------------+
			| constructors & destructor |
			+--------------------------*/

			using PrefixFilter::PrefixFilter;

			public:
			template <typename... Branches>
				explicit IndentFilter(
					const std::shared_ptr<IndentFilterState> &state,
					Branches &&...);

			/*----------------------------+
			| PrefixFilter implementation |
			+----------------------------*/

			private:
			std::string GetPrefix() const override final;

			/*-------------+
			| data members |
			+-------------*/

			private:
			std::shared_ptr<IndentFilterState> state =
				util::Share(GLOBAL(IndentFilterState));
		};
	}
}

#	include "IndentFilter.tpp"
#endif
