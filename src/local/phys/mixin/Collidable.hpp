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
