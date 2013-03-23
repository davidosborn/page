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

#include <algorithm> // max, min

#include "../math/interp.hpp" // HermiteScale
#include "Controller.hpp"

namespace page
{
	namespace phys
	{
		const float
			fadeInDuration  = .25,
			fadeOutDuration = .25,
			fadeExponent    = 1.5;

		// construct/destroy
		Controller::Controller(Layer layer) :
			layer(layer), alive(true), opacity(0) {}
		Controller::~Controller() {}

		// attributes
		bool Controller::IsAlive() const
		{
			return alive || opacity;
		}
		float Controller::GetOpacity() const
		{
			return math::HermiteScale(opacity, fadeExponent);
		}
		Controller::Layer Controller::GetLayer() const
		{
			return layer;
		}

		// update/generate frame
		void Controller::Update(float deltaTime)
		{
			Update(deltaTime, deltaTime);
		}
		void Controller::Update(float deltaTime, float deltaUpdateTime)
		{
			opacity = alive ?
				std::min(opacity + deltaTime / fadeInDuration,  1.f) :
				std::max(opacity - deltaTime / fadeOutDuration, 0.f);
			DoUpdate(deltaUpdateTime);
		}
		Frame Controller::GetFrame(const Frame &base, const Frame &accum) const
		{
			return DoGetFrame(base, accum);
		}

		// modifiers
		void Controller::Kill()
		{
			alive = false;
		}
		void Controller::SkipFade()
		{
			opacity = alive;
		}

		// dependencies
		Controller::Dependencies Controller::GetDependencies() const
		{
			return Dependencies();
		}

		// default update
		void Controller::DoUpdate(float) {}

		// layer comparison
		bool operator <(const Controller &c1, const Controller &c2)
		{
			return c1.GetLayer() < c2.GetLayer();
		}
		bool operator <(const Controller &c, Controller::Layer layer)
		{
			return c.GetLayer() < layer;
		}
	}
}
