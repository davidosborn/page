#ifndef    page_local_util_memory_ImplicitConversionClonerAdaptor_hpp
#   define page_local_util_memory_ImplicitConversionClonerAdaptor_hpp

#	include <functional> // function
#	include <memory> // unique_ptr

#	include "../algorithm/downcast.hpp"

namespace page { namespace util
{
	/**
	 * An adaptor for cloners to support implicit pointer conversion, such as
	 * derived-to-base-pointer conversion.
	 */
	template <typename T, typename U>
		struct ImplicitConversionClonerAdaptor
	{
		typedef std::unique_ptr<T> result_type;
		typedef const T &argument_type;

		private:
		typedef std::function<std::unique_ptr<U> (const U &)> cloner_type;

		public:
		explicit ImplicitConversionClonerAdaptor(const cloner_type &cloner) :
			cloner(cloner) {}

		std::unique_ptr<T> operator ()(const T &x) const
		{
			return cloner(downcast<const U &>(x));
		}

		private:
		cloner_type cloner;
	};
}}

#endif
