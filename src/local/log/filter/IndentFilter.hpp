/**
 * @section license
 *
 * Copyright (c) 2006-2012 David Osborn
 *
 * Permission is granted to use and redistribute this software in source and
 * binary form, with or without modification, subject to the following
 * conditions:
 *
 * 1. Redistributions in source form must retain the above copyright notice,
 *    this list of conditions, and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions, and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution, and in the same
 *    place and form as other copyright, license, and disclaimer information.
 *
 * As a special exception, distributions of derivative works in binary form may
 * include an acknowledgement in place of the above copyright notice, this list
 * of conditions, and the following disclaimer in the documentation and/or other
 * materials provided with the distribution, and in the same place and form as
 * other acknowledgements, similar in substance to the following:
 *
 *    Portions of this software are based on the work of David Osborn.
 *
 * This software is provided "as is", without any express or implied warranty.
 * In no event will the authors be liable for any damages arising out of the use
 * of this software.
 */

#ifndef    page_local_log_filter_IndentFilter_hpp
#   define page_local_log_filter_IndentFilter_hpp

#	include "../../util/class/inheritance.hpp" // INHERIT_CONSTRUCTORS
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

			public:
			INHERIT_CONSTRUCTORS(IndentFilter, PrefixFilter)

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
