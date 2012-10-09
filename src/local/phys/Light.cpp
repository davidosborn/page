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

#include <algorithm> // max
#include "Light.hpp"

namespace page
{
	namespace phys
	{
		// attenuation state
		float Light::GetMinRange() const
		{
			return std::max(GetRange() - GetFalloff() / 2, 0.f);
		}
		float Light::GetMaxRange() const
		{
			return GetRange() + GetFalloff() / 2;
		}

		// frame serialization
		Frame Light::GetFrame() const
		{
			return
				Ambient::GetFrame() +
				Attenuation::GetFrame() +
				Cutoff::GetFrame() +
				Diffuse::GetFrame() +
				Falloff::GetFrame() +
				PositionOrientation::GetFrame() +
				Range::GetFrame() +
				Specular::GetFrame();
		}
		void Light::Update(const Frame &frame)
		{
			Ambient::Update(frame);
			Attenuation::Update(frame);
			Cutoff::Update(frame);
			Diffuse::Update(frame);
			Falloff::Update(frame);
			PositionOrientation::Update(frame);
			Range::Update(frame);
			Specular::Update(frame);
		}
	}
}
