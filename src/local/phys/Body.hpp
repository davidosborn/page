#ifndef    page_local_phys_Body_hpp
#   define page_local_phys_Body_hpp

#	include "Form.hpp"
#	include "mixin/Collidable.hpp"

namespace page
{
	namespace phys
	{
		struct Body : Form, Collidable
		{
			// construct
			explicit Body(const cache::Proxy<res::Model> &);

			// FIXME: why does an ambiguity exist here?
			using Trackable::SetPosition;
		};
	}
}

#endif
