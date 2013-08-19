#ifndef    page_local_util_functional_cast_hpp
#   define page_local_util_functional_cast_hpp

namespace page
{
	namespace util
	{
		/**
		 * @defgroup functional-cast
		 *
		 * Function objects that wrap the type-cast operators.
		 *
		 * @{
		 */
#	define DEFINE_CAST_FUNCTION(TYPE) \
		template <typename T, typename U> \
			struct TYPE##_cast_function \
		{ \
			T operator ()(U x) const \
			{ \
				return TYPE##_cast<T>(x); \
			} \
		};

		DEFINE_CAST_FUNCTION(const)
		DEFINE_CAST_FUNCTION(dynamic)
		DEFINE_CAST_FUNCTION(reinterpret)
		DEFINE_CAST_FUNCTION(static)

#	undef DEFINE_CAST_FUNCTION
		///@}
	}
}

#endif
