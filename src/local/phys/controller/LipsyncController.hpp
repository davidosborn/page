// lip-sync controller
// expects bind-pose with closed mouth and neutral expression

#ifndef    page_local_phys_controller_LipsyncController_hpp
#   define page_local_phys_controller_LipsyncController_hpp

#	include <string>

#	include "TargetController.hpp"

namespace page { namespace phys
{
	namespace attrib { class Pose; }

	class LipsyncController :
		public TargetController,
		public virtual util::Cloneable<LipsyncController, TargetController>
	{
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
