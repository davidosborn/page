#ifndef    page_local_phys_mixin_update_Trackable_hpp
#   define page_local_phys_mixin_update_Trackable_hpp

namespace page
{
	namespace phys
	{
		// update
		template <typename Iterator>
			void UpdateTrackables(Iterator first, Iterator last);
	}
}

#	include "Trackable.tpp"
#endif
