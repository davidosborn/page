#ifndef    page_local_phys_controller_AnimationController_hpp
#   define page_local_phys_controller_AnimationController_hpp

#	include <string>
#	include <vector>

#	include "../../res/type/Animation.hpp" // Animation::{Bones,Vertices}
#	include "animation/Interpolator.hpp"
#	include "Controller.hpp"

namespace page { namespace phys
{
	class AnimationController : public Controller
	{
		IMPLEMENT_CLONEABLE(AnimationController, Controller)

		public:
		// construct
		explicit AnimationController(const res::Animation &, float timeScale = 1);

		// modifiers
		void SetPlayPosition(float);

		private:
		// update/generate frame
		void DoUpdate(float deltaTime);
		Frame DoGetFrame(const Frame &, const Frame &) const;

		float time, timeScale, duration;
		Interpolator<math::Vec3> position;
		Interpolator<math::Quat<>> orientation;
		Interpolator<math::Vec3> normal, scale;
		Interpolator<math::Vec2> texCoord;
		Interpolator<math::RgbColor<>> ambient, diffuse, specular;
		Interpolator<float> attenuation, cutoff, depth, exposure, falloff, fov, opacity, range, size, volume;
		struct Bone
		{
			// construct
			Bone(res::Animation::Bones::const_iterator::reference);

			std::string name;
			Interpolator<math::Vec3> position;
			Interpolator<math::Quat<>> orientation;
			Interpolator<math::Vec3> scale;
		};
		typedef std::vector<Bone> Bones;
		Bones bones;
		struct Vertex
		{
			// construct
			Vertex(res::Animation::Vertices::const_iterator::reference);

			unsigned index;
			Interpolator<math::Vec3> position, normal;
			Interpolator<math::Vec2> texCoord;
		};
		typedef std::vector<Vertex> Vertices;
		Vertices vertices;
	};
}}

#endif
