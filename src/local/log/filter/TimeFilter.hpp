/**
 * @copyright
 *
 * Copyright (c) 2006-2014 David Osborn
 *
 * Permission is granted to use and redistribute this software in source and
 * binary form, with or without modification, subject to the following
 * conditions:
 *
 * 1. Redistributions in source form must retain the above copyright notice,
 *    this list of conditions, and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions, and the following disclaimer in the same place
 *    and form as other copyright, license, and disclaimer information.
 *
 * 3. Redistributions in binary form must also include an acknowledgement in the
 *    same place and form as other acknowledgements (such as the credits),
 *    similar in substance to the following:
 *
 *       Portions of this software are based on the work of David Osborn.
 *
 * This software is provided "as is", without any express or implied warranty.
 * In no event will the authors be liable for any damages arising out of the use
 * of this software.
 */

#ifndef    page_local_log_filter_TimeFilter_hpp
#   define page_local_log_filter_TimeFilter_hpp

#	include "PrefixFilter.hpp"

namespace page
{
	namespace log
	{
		/**
		 * Prefixes each line with the current time.
		 */
		class TimeFilter final : public PrefixFilter
		{
			/*-------------+
			| constructors |
			+-------------*/

			using PrefixFilter::PrefixFilter;

			/*----------------------------+
			| PrefixFilter implementation |
			+----------------------------*/

			private:
			std::string GetPrefix() const override final;
		};
	}
}

#endif
