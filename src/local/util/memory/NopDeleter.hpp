#ifndef    page_local_util_memory_NopDeleter_hpp
#   define page_local_util_memory_NopDeleter_hpp

namespace page { namespace util
{
	/**
	 * A no-op deleter.
	 */
	template <typename T>
		struct NopDeleter
	{
		typedef void result_type;
		typedef const T *argument_type;

		void operator ()(const T *) const {}
	};

	/**
	 * A no-op non-const deleter.
	 */
	template <typename T>
		struct NopNonConstDeleter
	{
		typedef void result_type;
		typedef T *argument_type;

		void operator ()(T *) const {}
	};
}}

#endif
