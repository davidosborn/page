#ifndef    page_local_util_memory_DefaultDeleter_hpp
#   define page_local_util_memory_DefaultDeleter_hpp

namespace page { namespace util
{
	/**
	 * The default implementation of a deleter.
	 */
	template <typename T>
		struct DefaultDeleter
	{
		typedef void result_type;
		typedef const T *argument_type;

		void operator ()(const T *x) const
		{
			delete x;
		}
	};

	/**
	 * The default implementation of a non-const deleter.  This is the kind of
	 * deleter used by @c std::unique_ptr.
	 */
	template <typename T>
		struct DefaultNonConstDeleter
	{
		typedef void result_type;
		typedef T *argument_type;

		void operator ()(T *x) const
		{
			delete x;
		}
	};
}}

#endif
