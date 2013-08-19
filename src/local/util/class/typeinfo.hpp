#ifndef    page_local_util_class_typeinfo_hpp
#   define page_local_util_class_typeinfo_hpp

#	include <typeinfo> // type_info, typeid

namespace page
{
	namespace util
	{
		/**
		 * An alternative to @c typeid which permits the identification of
		 * incomplete types.
		 *
		 * @note This function returns an instance of @c std::type_info that is
		 *       not comparable with any external use of @c typeid.
		 */
		template <typename T>
			const std::type_info &GetIncompleteTypeInfo()
		{
			class Local {};
			return typeid (Local);
		}
	}
}

#endif
