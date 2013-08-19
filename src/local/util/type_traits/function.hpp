#ifndef    page_local_util_type_traits_function_hpp
#   define page_local_util_type_traits_function_hpp

namespace page
{
	namespace util
	{
		template <typename R, typename... Args>
			struct build_function_signature
		{
			typedef R type(Args...);
		};
	}
}

#endif
