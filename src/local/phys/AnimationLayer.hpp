#ifndef    page_local_phys_AnimationLayer_hpp
#   define page_local_phys_AnimationLayer_hpp

namespace page { namespace phys
{
	/**
	 * The available layers to which an animation may be attached.
	 *
	 * The layers are processed in order.  The enum values must be contiguous
	 * starting from zero, as they will be used to index an array.
	 */
	enum class AnimationLayer : unsigned
	{
		/**
		 * The controller will be applied immediately before performing
		 * collision detection and response.
		 */
		preCollision,

		/**
		 * The controller will be applied immediately after performing
		 * collision detection and response, and before applying any
		 * constraints.
		 */
		postCollision,

		/**
		 * The controller will be applied immediately after applying all
		 * constraints.
		 */
		postConstraint
	};
}}

#endif
