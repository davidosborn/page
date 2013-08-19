#ifndef    page_local_util_memory_NullCloner_hpp
#   define page_local_util_memory_NullCloner_hpp

#	include <memory> // unique_ptr

namespace page { namespace util
{
	/**
	 * A null cloner, which always returns @c nullptr.
	 */
	template <typename T>
		struct NullCloner
	{
		typedef std::unique_ptr<T> result_type;
		typedef const T &argument_type;

		std::unique_ptr<T> operator ()(const T &) const
		{
			return nullptr;
		}
	};
}}

#endif
