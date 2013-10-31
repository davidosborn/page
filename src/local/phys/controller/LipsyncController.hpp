#ifndef    page_local_phys_controller_LipsyncController_hpp
#   define page_local_phys_controller_LipsyncController_hpp

#	include <string>

#	include "TargetController.hpp"

namespace page { namespace phys
{
	namespace attrib { class Pose; }

	/**
	 * Lip-sync controller.
	 *
	 * @note Expects a bind-pose with closed mouth and neutral expression.
	 */
	class LipsyncController : public TargetController
	{
		IMPLEMENT_CLONEABLE(LipsyncController, Controller)

		public:
		// construct
		explicit LipsyncController(const attrib::Pose &);

		// modifiers
		void PushText(const std::string &);
		void PushPhonemes(const std::string &);

		// check compatibility
		static bool Check(const attrib::Pose &);

		private:
		// update targets
		void UpdateTargets(float deltaTime);

		std::string phonemes;
		float phonemeDuration;
	};
}}

#endif
