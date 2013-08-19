#ifndef    page_local_phys_controller_HeroCamController_hpp
#   define page_local_phys_controller_HeroCamController_hpp

#	include "../../cache/proxy/Aabb.hpp"
#	include "../../math/Euler.hpp"
#	include "../Controller.hpp"

namespace page { namespace inp { class Driver; }}

namespace page { namespace phys
{
	class Form;

	class HeroCamController :
		public Controller,
		public virtual util::Cloneable<HeroCamController, Controller>
	{
		public:
		// construct
		HeroCamController(const inp::Driver &, const Form &);

		// modifiers
		void SetTarget(const Form &);

		// dependencies
		Dependencies GetDependencies() const;

		private:
		// generate frame
		void DoUpdate(float deltaTime);
		Frame DoGetFrame(const Frame &, const Frame &) const;

		const inp::Driver &driver;
		cache::Aabb aabb;
		math::Euler<> orientation;
		float center, distance;
		Dependencies dependencies;
	};
}}

#endif
