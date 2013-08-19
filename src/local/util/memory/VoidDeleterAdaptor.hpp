#ifndef    page_local_util_memory_VoidDeleterAdaptor_hpp
#   define page_local_util_memory_VoidDeleterAdaptor_hpp

#	include <functional> // function

namespace page { namespace util
{
	/**
	 * An adaptor to allow a deleter taking a typed pointer to be stored in a
	 * pointer to a function taking a @c void pointer.
	 */
	template <typename T>
		struct VoidDeleterAdaptor
	{
		typedef void result_type;
		typedef const T *argument_type;

		private:
		typedef std::function<void (const T *)> deleter_type;

		public:
		explicit VoidDeleterAdaptor(const deleter_type &deleter) :
			deleter(deleter) {}

		void operator ()(const void *x) const
		{
			return deleter(static_cast<const T *>(x));
		}

		private:
		deleter_type deleter;
	};
}}

#endif
