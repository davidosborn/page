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

#ifndef    page_local_util_functional_compare_hpp
#   define page_local_util_functional_compare_hpp

#	include <functional> // {,not}equal_to, {greater,less}{,_equal}
#	include <utility> // declval

namespace page
{
	namespace util
	{
		/**
		 * @defgroup functional-compare-indirect
		 *
		 * Function objects providing indirect versions of @c std::less and
		 * friends.
		 *
		 * @{
		 */
#	define DEFINE_INDIRECT_COMPARE_FUNCTION(TYPE) \
		template <typename T> \
			struct indirect_##TYPE \
		{ \
			bool operator ()(const T &a, const T &b) \
			{ \
				return std::TYPE<decltype(*std::declval<T>())>()(*a, *b); \
			} \
		}; \
		template <typename T> \
			struct double_indirect_##TYPE \
		{ \
			bool operator ()(const T &a, const T &b) \
			{ \
				return std::TYPE<decltype(**std::declval<T>())>()(**a, **b); \
			} \
		};

		DEFINE_INDIRECT_COMPARE_FUNCTION(equal_to)
		DEFINE_INDIRECT_COMPARE_FUNCTION(not_equal_to)
		DEFINE_INDIRECT_COMPARE_FUNCTION(greater)
		DEFINE_INDIRECT_COMPARE_FUNCTION(less)
		DEFINE_INDIRECT_COMPARE_FUNCTION(greater_equal)
		DEFINE_INDIRECT_COMPARE_FUNCTION(less_equal)

#	undef DEFINE_INDIRECT_CMP
		///@}
	}
}

#endif
