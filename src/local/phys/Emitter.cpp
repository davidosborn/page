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

#include "Emitter.hpp"

namespace page
{
	namespace phys
	{
		// update
		void Emitter::Update()
		{
			// FIXME: create particles
		}

		// frame serialization
		Frame Emitter::GetFrame() const
		{
			return
				AmbientRange::GetFrame() +
				Cutoff::GetFrame() +
				DiffuseRange::GetFrame() +
				EmissiveRange::GetFrame() +
				LifetimeRange::GetFrame() +
				OpacityRange::GetFrame() +
				PositionOrientation::GetFrame() +
				SpecularRange::GetFrame() +
				SizeRange::GetFrame() +
				SpeedRange::GetFrame();
		}
		void Emitter::Update(const Frame &frame)
		{
			AmbientRange::Update(frame);
			Cutoff::Update(frame);
			DiffuseRange::Update(frame);
			EmissiveRange::Update(frame);
			LifetimeRange::Update(frame);
			OpacityRange::Update(frame);
			PositionOrientation::Update(frame);
			SpecularRange::Update(frame);
			SizeRange::Update(frame);
			SpeedRange::Update(frame);
		}
	}
}
