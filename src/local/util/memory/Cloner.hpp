#ifndef    page_local_util_memory_Cloner_hpp
#   define page_local_util_memory_Cloner_hpp

#	include <functional> // function
#	include <memory> // unique_ptr

namespace page { namespace util
{
	/**
	 * A cloner function, for use with smart pointers.
	 */
	template <typename T>
		using Cloner = std::function<std::unique_ptr<T> (const T &)>;
}}

#endif
