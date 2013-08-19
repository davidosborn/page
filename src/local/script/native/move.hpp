// FIXME: should probably make this module more general

#ifndef    page_local_script_native_move_hpp
#   define page_local_script_native_move_hpp

#	include <string>

#	include "../../game/Character.hpp"
#	include "../../math/fwd.hpp" // Vector

namespace page
{
	namespace script
	{
		// camera
		int GetCameraView();
		void SetCameraView(int);
		void SetCamera

		// movement
		enum Gait
		{
			crawlGait,
			runGait,
			sneakGait,
			walkGait
		};
		bool IsMoving();
		Gait GetGait();
		void Move(game::Character &, const math::Vec2 &, Gait = walkGait);
		void Move(game::Character &, const math::Vec2 &, const res::Track::Face &, Gait = walkGait);
		void Move(game::Character &, const math::Vec3 &, Gait = walkGait);

		bool IsSitting(game::Character &);
		void ToggleSit(game::Character &);

		// interaction
		void Say(game::Character &, const std::string &);
	}
}

#endif
