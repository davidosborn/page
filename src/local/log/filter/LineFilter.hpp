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

#ifndef    page_local_log_filter_LineFilter_hpp
#   define page_local_log_filter_LineFilter_hpp

#	include "Filter.hpp"

namespace page
{
	namespace log
	{
		/**
		 * The base class for filters which process newline-terminated lines,
		 * rather than individual characters.
		 */
		class LineFilter : public Filter
		{
			/*-------------+
			| constructors |
			+-------------*/

			using Filter::Filter;

			/*------------------+
			| virtual functions |
			+------------------*/

			private:
			virtual std::string FilterLine(const std::string &) const = 0;

			/*----------------------+
			| Filter implementation |
			+----------------------*/

			private:
			std::string DoFilter(const std::string &) const override final;

			/*----------------------+
			| Stream implementation |
			+----------------------*/

			private:
			void DoClear() override;

			/*-------------+
			| data members |
			+-------------*/

			private:
			/**
			 * A buffer holding the last incomplete line.  A line is complete
			 * when it is terminated by a newline character.
			 */
			mutable std::string buffer;
		};
	}
}

#endif
