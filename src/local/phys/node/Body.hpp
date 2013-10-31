#ifndef    page_local_phys_node_Body_hpp
#   define page_local_phys_node_Body_hpp

#	include "../mixin/Collidable.hpp"
#	include "Form.hpp"

namespace page { namespace phys
{
	/**
	 * A body node, which is a collidable form.
	 */
	class Body :
		public Form,
		public Collidable
	{
		IMPLEMENT_CLONEABLE(Body, Node)

		public:
		// construct
		explicit Body(const cache::Proxy<res::Model> &);

		// FIXME: why does an ambiguity exist here?
		using Trackable::SetPosition;
	};
}}

#endif
