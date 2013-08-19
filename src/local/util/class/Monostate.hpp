#ifndef    page_local_util_class_Monostate_hpp
#   define page_local_util_class_Monostate_hpp

namespace page
{
	namespace util
	{
		template <typename T>
			class Monostate
		{
			public:
			/**
			 * @return A reference to a single global instance of the class.
			 */
			static T &GetGlobalInstance()
			{
				static T instance;
				return instance;
			}
		};
	}
}

#	define GLOBAL(x) (x::GetGlobalInstance())

#endif
