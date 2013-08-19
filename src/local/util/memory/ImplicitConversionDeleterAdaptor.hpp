#ifndef    page_local_util_memory_ImplicitConversionDeleterAdaptor_hpp
#   define page_local_util_memory_ImplicitConversionDeleterAdaptor_hpp

#	include <functional> // function

#	include "../algorithm/downcast.hpp"

namespace page { namespace util
{
	/**
	 * An adaptor for deleters to support implicit pointer conversion, such as
	 * derived-to-base-pointer conversion.
	 */
	template <typename T, typename U>
		struct ImplicitConversionDeleterAdaptor
	{
		typedef void result_type;
		typedef const T *argument_type;

		private:
		typedef std::function<void (const U *)> deleter_type;

		public:
		explicit ImplicitConversionDeleterAdaptor(const deleter_type &deleter) :
			deleter(deleter) {}

		void operator ()(const T *x) const
		{
			return deleter(&downcast<const U &>(*x));
		}

		private:
		deleter_type deleter;
	};
}}

#endif
