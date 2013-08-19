#ifndef    page_local_phys_mixin_update_Collidable_hpp
#   define page_local_phys_mixin_update_Collidable_hpp

namespace page
{
	namespace phys
	{
		// update
		template <typename Iterator>
			void UpdateCollidables(Iterator first, Iterator last);
	}
}

#	include "Collidable.tpp"
#endif
