#ifndef    page_local_util_memory_Deleter_hpp
#   define page_local_util_memory_Deleter_hpp

#	include <functional> // function

namespace page { namespace util
{
	/**
	 * A pointer to a deleter function, for use with smart pointers.
	 */
	template <typename T>
		using Deleter = std::function<void (const T *)>;

	/**
	 * A pointer to a deleter function, for use with smart pointers.
	 *
	 * This variation accepts a non-const pointer, which is compatible with the
	 * C++ Standard Library, such as in the case of @c std::default_delete.
	 */
	template <typename T>
		using NonConstDeleter = std::function<void (T *)>;
}}

#endif
