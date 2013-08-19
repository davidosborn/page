#ifndef    page_local_util_type_traits_class_template_hpp
#   define page_local_util_type_traits_class_template_hpp

#	include <type_traits>

namespace page { namespace util
{
	/**
	 * A metafunction which determines whether a type is a specialization of
	 * a class template.
	 */
	template <typename, template <typename...> class>
		struct is_specialization_of :
			std::false_type {};

	template <template <typename...> class Template, typename... Args>
		struct is_specialization_of<Template<Args...>, Template> :
			std::true_type {};
}}

#endif
