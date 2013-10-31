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
