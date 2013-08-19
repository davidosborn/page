#ifndef    page_local_phys_mixin_Transformable_hpp
#   define page_local_phys_mixin_Transformable_hpp

#	include <boost/signal.hpp>

namespace page
{
	namespace phys
	{
		struct Transformable
		{
			// destroy
			virtual ~Transformable();

			// delta modifiers
			virtual void BakeTransform() = 0;
			virtual void UpdateForce() = 0;
			virtual void UpdateDelta() = 0;

			// signals
			boost::signal<void ()> dirtyTransformSig;
		};
	}
}

#endif
