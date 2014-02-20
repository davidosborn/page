/**
 * @copyright
 *
 * Copyright (c) 2006-2014 David Osborn
 *
 * Permission is granted to use and redistribute this software in source and
 * binary form, with or without modification, subject to the following
 * conditions:
 *
 * 1. Redistributions in source form must retain the above copyright notice,
 *    this list of conditions, and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions, and the following disclaimer in the same place
 *    and form as other copyright, license, and disclaimer information.
 *
 * 3. Redistributions in binary form must also include an acknowledgement in the
 *    same place and form as other acknowledgements (such as the credits),
 *    similar in substance to the following:
 *
 *       Portions of this software are based on the work of David Osborn.
 *
 * This software is provided "as is", without any express or implied warranty.
 * In no event will the authors be liable for any damages arising out of the use
 * of this software.
 */

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
