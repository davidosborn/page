#ifndef    page_system_boost_mpl_unpack_hpp
#   define page_system_boost_mpl_unpack_hpp

#	include <boost/mpl/back.hpp>
#	include <boost/mpl/empty.hpp>
#	include <boost/mpl/pop_back.hpp>

namespace boost { namespace mpl
{
	/**
	 * Instantiates a class template, unpacking a sequence into the
	 * template arguments.
	 *
	 * @note This class has been structured like a
	 *       <em>metafunction class</em> to maximize its usability.
	 */
	template <template <typename...> class Template>
		struct unpack
	{
		template <
			typename Sequence,
			bool = empty<Sequence>::value,
			typename... Args>
				struct apply
		{
			typedef
				typename apply<
					typename pop_back<Sequence>::type,
					empty<typename pop_back<Sequence>::type>::value,
					typename back<Sequence>::type,
					Args...>::type type;
		};

		template <typename Sequence, typename... Args>
			struct apply<Sequence, true, Args...>
		{
			typedef Template<Args...> type;
		};
	};
}}

#endif
