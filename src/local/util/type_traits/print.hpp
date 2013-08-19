#ifndef    page_local_util_type_traits_print_hpp
#   define page_local_util_type_traits_print_hpp

namespace page
{
	namespace util
	{
		/**
		 * Call this with a complex type and have GCC print it out.
		 *
		 * Thanks to Johannes Schaub (litb).
		 * http://stackoverflow.com/a/596348
		 */
		template <typename T>
			std::string PrintType()
		{
			return __PRETTY_FUNCTION__;
		}
	}
}

#endif
