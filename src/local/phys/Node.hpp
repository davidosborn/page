#ifndef    page_local_phys_Node_hpp
#   define page_local_phys_Node_hpp

#	include "../math/Aabb.hpp"

namespace page
{
	namespace phys
	{
		struct Node
		{
			explicit Node(const math::Aabb<> & = math::Aabb<>());

			const math::Aabb<> &GetAabb() const;
			void SetAabb(const math::Aabb<> &);

			private:
			math::Aabb<> aabb;
		};
	}
}

#endif
