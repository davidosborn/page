#ifndef    page_local_phys_controller_FollowController_hpp
#   define page_local_phys_controller_FollowController_hpp

#	include "../../cache/proxy/Proxy.hpp"
#	include "../../math/fwd.hpp" // Aabb
#	include "../../math/Quat.hpp"
#	include "../../math/Vector.hpp"
#	include "../Controller.hpp"

namespace page { namespace phys
{
	class Form;

	class FollowController :
		public Controller,
		public virtual util::Cloneable<FollowController, Controller>
	{
		public:
		// construct
		explicit FollowController(
			const Form &target,
			const math::Vec3 &center = .5,
			const math::Quat<> &orientation = math::Quat<>(),
			float distance = 0);

		// modifiers
		void Follow(const Form &target);
		void Follow(const Form &target, const math::Vec3 &center);
		void Reorient(const math::Quat<> &orientation);
		void Reorient(const math::Quat<> &orientation, float distance);

		// dependencies
		Dependencies GetDependencies() const;

		private:
		// generate frame
		Frame DoGetFrame(const Frame &, const Frame &) const;

		cache::Proxy<math::Aabb<3>> aabb;
		math::Vec3 center;
		math::Quat<> orientation;
		float distance;
		Dependencies dependencies;
	};
}}

#endif
