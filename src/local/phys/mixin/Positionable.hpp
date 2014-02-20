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

#ifndef    page_local_phys_mixin_Positionable_hpp
#   define page_local_phys_mixin_Positionable_hpp

#	include "../../math/fwd.hpp" // Vector
#	include "../../util/class/special_member_functions.hpp" // Polymorphic

namespace page { namespace phys
{
	/**
	 * A base class for position-related attributes, which specifies an
	 * interface for third parties to access position-related information.
	 *
	 * @deprecated Replace with attrib::Position.
	 */
	class Positionable : public util::Polymorphic<Positionable>
	{
		public:
		/**
		 * Returns the node's current position.
		 */
		virtual const math::Vec3 &GetPosition() const = 0;

		/**
		 * Sets the node's position.
		 */
		virtual void SetPosition(const math::Vec3 &) = 0;

		/**
		 * Returns the node's previous position.
		 */
		virtual const math::Vec3 &GetLastPosition() const = 0;

		/**
		 * Returns the node's current velocity, calculated from its current and
		 * previous positions.
		 *
		 * @deprecated Not currently used.
		 */
		math::Vec3 GetVelocity() const;
	};
}}

#endif
