// locomotion controller
// affects top-level position/orientation attributes
// supports either targeted movement or discrete velocity/rotation forces

#ifndef    page_local_phys_controller_LocomotionController_hpp
#   define page_local_phys_controller_LocomotionController_hpp

#	include <boost/optional.hpp>

#	include "../../math/Euler.hpp"
#	include "../../math/Quat.hpp"
#	include "../../math/Vector.hpp"
#	include "../Controller.hpp"

namespace page { namespace phys
{
	namespace attrib { class PositionOrientation; }

	class LocomotionController :
		public Controller,
		public virtual util::Cloneable<LocomotionController, Controller>
	{
		public:
		// construct
		explicit LocomotionController(const attrib::PositionOrientation &);

		// target modifiers
		bool HasTarget() const;
		void SetTarget();
		void SetTarget(const math::Vec3 &position);
		void SetTarget(const math::Quat<> &orientation);
		void SetTarget(const math::Vec3 &position, const math::Quat<> &orientation);

		// force modifiers
		void SetForce(const math::Vec3 &velocity);
		void SetForce(const math::Euler<> &rotation);
		void SetForce(const math::Vec3 &velocity, const math::Euler<> &rotation);

		protected:
		// update hooks
		virtual void UpdateLocomotion();

		private:
		// update/generate frame
		void DoUpdate(float deltaTime);
		Frame DoGetFrame(const Frame &, const Frame &) const;

		const attrib::PositionOrientation &controlled;

		// target
		boost::optional<math::Vec3> position;
		boost::optional<math::Quat<>> orientation;

		// force
		math::Vec3 velocity, translation;
		math::Euler<> angularVelocity, rotation;
	};
}}

#endif
