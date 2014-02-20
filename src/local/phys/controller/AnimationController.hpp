/**
 * @copyright
 *
 * Copyright (c) 2006-2014 David Osborn
 *
 * Permission is granted to use and redistribute this software in source and
 * binary form, with or without modification, subject to the following
 * conditions:
 *
 * 1. Redistributions in source form must retain the above copyright notice,
 *    this list of conditions, and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions, and the following disclaimer in the same place
 *    and form as other copyright, license, and disclaimer information.
 *
 * 3. Redistributions in binary form must also include an acknowledgement in the
 *    same place and form as other acknowledgements (such as the credits),
 *    similar in substance to the following:
 *
 *       Portions of this software are based on the work of David Osborn.
 *
 * This software is provided "as is", without any express or implied warranty.
 * In no event will the authors be liable for any damages arising out of the use
 * of this software.
 */

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

		/*-------------+
		| constructors |
		+-------------*/

		public:
		explicit AnimationController(const res::Animation &, float timeScale = 1);

		/*----------+
		| modifiers |
		+----------*/

		void SetPlayPosition(float);

		/*--------------------------+
		| Controller implementation |
		+--------------------------*/

		private:
		void DoUpdate(float deltaTime);
		Frame DoGetFrame(const Frame &, const Frame &) const;

		/*-------------+
		| data members |
		+-------------*/

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
