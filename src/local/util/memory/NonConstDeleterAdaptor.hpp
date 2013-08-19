#ifndef    page_local_util_memory_NonConstDeleterAdaptor_hpp
#   define page_local_util_memory_NonConstDeleterAdaptor_hpp

#	include <functional> // function

namespace page { namespace util
{
	/**
	 * An adaptor to allow a deleter taking a non-const pointer to be stored in
	 * a pointer to a function taking a const pointer.
	 */
	template <typename T>
		struct NonConstDeleterAdaptor
	{
		typedef void result_type;
		typedef const T *argument_type;

		private:
		typedef std::function<void (T *)> deleter_type;

		public:
		explicit NonConstDeleterAdaptor(const deleter_type &deleter) :
			deleter(deleter) {}

		void operator ()(const T *x) const
		{
			return deleter(const_cast<T *>(x));
		}

		private:
		deleter_type deleter;
	};
}}

#endif
