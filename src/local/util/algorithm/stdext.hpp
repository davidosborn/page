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
