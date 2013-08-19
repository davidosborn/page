#include "Node.hpp"

namespace page
{
	namespace phys
	{
		Node::Node(const math::Aabb &aabb) : aabb(aabb) {}

		const math::Aabb &Node::GetAabb() const
		{
			return aabb;
		}
		void Node::SetAabb(const math::Aabb &aabb)
		{
			this->aabb = aabb;
		}
	}
}
