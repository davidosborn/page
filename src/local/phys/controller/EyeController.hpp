#ifndef    page_local_phys_controller_EyeController_hpp
#   define page_local_phys_controller_EyeController_hpp

#	include "../../math/Vector.hpp"
#	include "Controller.hpp"

namespace page { namespace phys
{
	class Positionable;
	namespace attrib { class Pose; }

	class EyeController : public Controller
	{
		IMPLEMENT_CLONEABLE(EyeController, Controller)

		public:
		// construct
		explicit EyeController(const attrib::Pose &);

		// modifiers
		void Reset();
		void LookAt(const math::Vec3 &);
		void Follow(const Positionable &, const math::Vec3 & = 0);

		// dependencies
		Dependencies GetDependencies() const;

		// check compatibility
		static bool Check(const attrib::Pose &);

		private:
		// generate frame
		Frame DoGetFrame(const Frame &, const Frame &) const;

		enum Mode
		{
			noMode,
			lookAtMode,
			followMode
		} mode;
		struct LookAt
		{
			math::Vec3 position;
		} lookAt;
		struct Follow
		{
			const Positionable *target;
			math::Vec3 position;
		} follow;
		Dependencies dependencies;
	};
}}

#endif
