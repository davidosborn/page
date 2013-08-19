#ifndef    page_local_util_memory_DefaultCloner_hpp
#   define page_local_util_memory_DefaultCloner_hpp

#	include <memory> // unique_ptr

namespace page { namespace util
{
	/**
	 * The default implementation of a cloner.
	 */
	template <typename T>
		struct DefaultCloner
	{
		typedef std::unique_ptr<T> result_type;
		typedef const T &argument_type;

		std::unique_ptr<T> operator ()(const T &x) const
		{
			return std::unique_ptr<T>(new T(x));
		}
	};
}}

#endif
