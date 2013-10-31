#ifndef    page_local_phys_mixin_Transformable_hpp
#   define page_local_phys_mixin_Transformable_hpp

#	include "../../util/class/special_member_functions.hpp" // Polymorphic
#	include "../../util/copyable_signal.hpp"

namespace page { namespace phys
{
	/**
	 * A mixin which makes the derived attribute transformable, allowing for
	 * interoperability with the rest of the physics system.
	 */
	class Transformable : public util::Polymorphic<Transformable>
	{
		public:
		Transformable() = default;
		Transformable(const Transformable &);

		/**
		 * Resets the transformation from the last frame to match the current
		 * transformation, thereby zeroing out any calculated force/delta.
		 */
		virtual void BakeTransform() = 0;

		/**
		 * Updates the current force using the difference between the current
		 * transformation and the transformation from the last frame.
		 */
		virtual void UpdateForce() = 0;

		/**
		 * Updates the delta transformation to the difference between the
		 * current transformation and the transformation from the last frame.
		 */
		virtual void UpdateDelta() = 0;

		/**
		 * A signal that will be called when the transformation changes.  It is
		 * typically used to invalidate any cached data that is based on the
		 * transformation.
		 */
		util::copyable_signal<void ()> dirtyTransformSig;
	};
}}

#endif
