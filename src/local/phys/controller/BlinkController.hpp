#ifndef    page_local_phys_controller_BlinkController_hpp
#   define page_local_phys_controller_BlinkController_hpp

#	include <random> // mt19937

#	include "../../math/Quat.hpp"
#	include "Controller.hpp"

namespace page { namespace phys
{
	namespace attrib { class Pose; }

	/**
	 * Controls the natural blinking of a character's eyes.
	 *
	 * @note The controller expects the character to have their eyes closed in
	 * their bind pose.
	 */
	class BlinkController : public Controller
	{
		IMPLEMENT_CLONEABLE(BlinkController, Controller)

		public:
		// construct
		explicit BlinkController(const attrib::Pose &);

		// check compatibility
		static bool Check(const attrib::Pose &);

		private:
		// update/generate frame
		void DoUpdate(float deltaTime);
		Frame DoGetFrame(const Frame &, const Frame &) const;

		enum State
		{
			openState,
			closingState,
			closedState,
			openingState
		} state;
		float open, delay;
		math::Quat<>
			leftEyelidBindOrientation, rightEyelidBindOrientation,
			leftEyelidOpenOrientation, rightEyelidOpenOrientation;
		std::mt19937 rand;
	};
}}

#endif
