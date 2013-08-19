#ifndef    page_local_util_locale_UsableFacet_hpp
#   define page_local_util_locale_UsableFacet_hpp

#	include "../class/special_member_functions.hpp" // Uncopyable

namespace page
{
	namespace util
	{
		/**
		 * An adaptor for the Standard Library's facets, to circumvent their
		 * protected destructor.
		 *
		 * @sa Based on <a href="http://stackoverflow.com/a/7599989">Howard
		 *     Hinnant's solution</a>.
		 */
		template <typename Facet>
			class UsableFacet :
				public Facet,
				public Uncopyable<UsableFacet<Facet>>
		{
			using Facet::Facet;

			public:
			~UsableFacet() = default;
		};
	}
}

#endif
