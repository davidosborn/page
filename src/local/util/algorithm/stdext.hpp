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

#ifndef    page_local_util_algorithm_stdext_hpp
#   define page_local_util_algorithm_stdext_hpp

namespace page
{
	namespace util
	{
		/*-------------------+
		| copy_{until,while} |
		+-------------------*/

		template <typename InputIterator, typename OutputIterator, typename Predicate>
			OutputIterator copy_until(InputIterator first, InputIterator last, OutputIterator result, Predicate pred)
		{
			while (first != last && !pred(*first))
				*result++ = *first++;
			return result;
		}

		template <typename InputIterator, typename OutputIterator, typename Predicate>
			OutputIterator copy_while(InputIterator first, InputIterator last, OutputIterator result, Predicate pred)
		{
			while (first != last && pred(*first))
				*result++ = *first++;
			return result;
		}

		/*---------+
		| find_not |
		+---------*/

		template <typename InputIterator, typename T>
			InputIterator find_not(InputIterator first, InputIterator last, const T &value)
		{
			while (first != last && *first == value) ++first;
			return first;
		}

		/*------------+
		| for_each_if |
		+------------*/

		template <typename InputIterator, typename Function, typename Predicate>
			Function for_each_if(InputIterator first, InputIterator last, Function func, Predicate pred)
		{
			for (; first != last; ++first)
				if (pred(*first)) func(*first);
			return func;
		}

		/*------------+
		| transform_n |
		+------------*/

		template <typename InputIterator, typename Size, typename OutputIterator, typename UnaryOperation>
			OutputIterator transform_n(InputIterator first, Size count, OutputIterator result, UnaryOperation op)
		{
			while (count--) *result++ = op(*first++);
			return result;
		}

		template <typename InputIterator1, typename Size, typename InputIterator2, typename OutputIterator, typename BinaryOperation>
			OutputIterator transform_n(InputIterator1 first1, Size count, InputIterator2 first2, OutputIterator result, BinaryOperation op)
		{
			while (count--) *result++ = op(*first1++, *first2++);
			return result;
		}
	}
}

#endif
