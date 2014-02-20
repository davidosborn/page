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

#ifndef    page_local_phys_controller_animation_Interpolator_hpp
#   define page_local_phys_controller_animation_Interpolator_hpp

#	include "../../../res/type/Animation.hpp" // Animation::Channel

namespace page
{
	namespace phys
	{
		/**
		 * Key frame interpolator.
		 *
		 * @note Inspired by CatMother's anim::Interpolator.
		 */
		template <typename T> struct Interpolator
		{
			enum InterpolationType
			{
				stepInterpolation,
				linearInterpolation,
				cubicInterpolation
			};

			// constructor
			// TEST: cubic interpolation should be the default
			Interpolator(const res::Animation::Channel<T> &, InterpolationType = linearInterpolation);

			// interpolated access
			T Get(float time) const;

			// validity
			explicit operator bool() const;

			private:
			// frame
			struct Frame
			{
				// construct
				explicit Frame(float time, const T & = T());
				Frame(const typename res::Animation::Channel<T>::Frame &);

				float time;
				T value;
			};
			typedef std::vector<Frame> Frames;

			// bounded frame search
			typename Frames::const_iterator FindFloorFrame(float time) const;

			// bounded frame iteration
			typename Frames::const_iterator Advance(const typename Frames::const_iterator &, int) const;

			// frame comparison
			static bool CompareFrameTime(const Frame &, const Frame &);

			Frames frames;
			InterpolationType type;
		};
	}
}

#	include "Interpolator.tpp"
#endif
