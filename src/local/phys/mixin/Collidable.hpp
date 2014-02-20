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

#ifndef    page_local_phys_mixin_Collidable_hpp
#   define page_local_phys_mixin_Collidable_hpp

#	include "Trackable.hpp"

namespace page { namespace phys
{
	/**
	 * A mixin which makes the derived node collidable.  Collidable nodes are
	 * capable of being involved in collision detection and response on the
	 * track.
	 */
	class Collidable : public Trackable
	{
		/*------+
		| types |
		+------*/

		public:
		/**
		 * The available types of colliders.
		 *
		 * @note Track walls count as passive colliders.
		 */
		enum class Type
		{
			/**
			 * Collidable against active and passive colliders.
			 */
			active,

			/**
			 * Collidable against active colliders.
			 */
			passive,

			/**
			 * Not collidable.
			 */
			inactive
		};

		/*-------------+
		| constructors |
		+-------------*/

		explicit Collidable(Type = Type::inactive, float radius = 1);

		/*----------+
		| accessors |
		+----------*/

		Type GetType() const;
		void SetType(Type);

		float GetRadius() const;
		void SetRadius(float);

		/*-------------+
		| data members |
		+-------------*/

		private:
		/**
		 * The type of the collider.
		 */
		Type type;

		/**
		 * The radius of the collider.
		 */
		float radius;
	};
}}

#endif
