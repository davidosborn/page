/**
 * @section license
 *
 * Copyright (c) 2006-2013 David Osborn
 *
 * Permission is granted to use and redistribute this software in source and
 * binary form, with or without modification, subject to the following
 * conditions:
 *
 * 1. Redistributions in source form must retain the above copyright notice,
 *    this list of conditions, and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions, and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution, and in the same
 *    place and form as other copyright, license, and disclaimer information.
 *
 * As a special exception, distributions of derivative works in binary form may
 * include an acknowledgement in place of the above copyright notice, this list
 * of conditions, and the following disclaimer in the documentation and/or other
 * materials provided with the distribution, and in the same place and form as
 * other acknowledgements, similar in substance to the following:
 *
 *    Portions of this software are based on the work of David Osborn.
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
#	include "../Controller.hpp"
#	include "animation/Interpolator.hpp"

namespace page { namespace phys
{
	class AnimationController :
		public Controller,
		public virtual util::Cloneable<AnimationController, Controller>
	{
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
		Interpolator<math::Vector<3>> position;
		Interpolator<math::Quat<>> orientation;
		Interpolator<math::Vector<3>> normal, scale;
		Interpolator<math::Vector<2>> texCoord;
		Interpolator<math::RgbColor<>> ambient, diffuse, specular;
		Interpolator<float> attenuation, cutoff, depth, exposure, falloff, fov, opacity, range, size, volume;
		struct Bone
		{
			// construct
			Bone(res::Animation::Bones::const_iterator::reference);

			std::string name;
			Interpolator<math::Vector<3>> position;
			Interpolator<math::Quat<>> orientation;
			Interpolator<math::Vector<3>> scale;
		};
		typedef std::vector<Bone> Bones;
		Bones bones;
		struct Vertex
		{
			// construct
			Vertex(res::Animation::Vertices::const_iterator::reference);

			unsigned index;
			Interpolator<math::Vector<3>> position, normal;
			Interpolator<math::Vector<2>> texCoord;
		};
		typedef std::vector<Vertex> Vertices;
		Vertices vertices;
	};
}}

#endif
