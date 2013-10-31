#include "Body.hpp"

namespace page
{
	namespace phys
	{
		// construct
		Body::Body(const cache::Proxy<res::Model> &model) : Form(model)
		{
			Trackable::Init();
		}
	}
}
