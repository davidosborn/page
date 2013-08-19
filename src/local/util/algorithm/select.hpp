#ifndef    page_local_util_algorithm_select_hpp
#   define page_local_util_algorithm_select_hpp

namespace page
{
	namespace util
	{
		/*----------+
		| selection |
		+----------*/

		template <typename InputIterator1, typename InputIterator2, typename InputIterator3, typename OutputIterator>
			OutputIterator select(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator3 first3, OutputIterator result)
		{
			for (; first1 != last1; ++first2, ++first3)
				*result++ = *first1++ ? *first2 : *first3;
			return result;
		}

		template <typename InputIterator1, typename InputIterator2, typename InputIterator3, typename OutputIterator, typename TrinaryOperation>
			OutputIterator select(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator3 first3, OutputIterator result, TrinaryOperation op)
		{
			while (first1 != last1)
				*result++ = op(*first1++, *first2++, *first3++);
			return result;
		}
	}
}

#endif
