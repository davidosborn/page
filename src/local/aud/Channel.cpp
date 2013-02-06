/**
 * @section license
 *
 * Copyright (c) 2006-2012 David Osborn
 *
 * Permission is granted to use and redistribute this software in source and
 * binary form, with or without modification, subject to the following
 * conditions:
 *
 * 1. Redistributions in source form must retain the above copyright notice,
 *    this list of conditions, and the following disclaimer.

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

#include <algorithm> // min
#include "../math/float.hpp" // LinearToDecibel
#include "../math/interp.hpp" // HermiteScale
#include "Channel.hpp"

namespace page
{
	namespace aud
	{
		const float
			fadeInDuration = 1,
			Channel::fadeExponent = 1.5;

		// construct/destroy
		Channel::Channel(bool fade) : level(!fade) {}
		Channel::~Channel() {}

		// update
		void Channel::Update(float deltaTime)
		{
			level = std::min(level + deltaTime / fadeInDuration, 1.f);
			DoUpdate(deltaTime);
			DoUpdate2();
		}

		// state
		float Channel::GetVolume() const
		{
			return math::LinearToDecibel(GetLinearVolume());
		}
		float Channel::GetLinearVolume() const
		{
			return DoGetLinearVolume() *
				math::HermiteScale(level, fadeExponent);
		}
	}
}
