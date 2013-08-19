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
