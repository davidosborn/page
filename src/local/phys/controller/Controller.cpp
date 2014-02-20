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

#include <algorithm> // max, min

#include "../../math/interp.hpp" // HermiteScale
#include "Controller.hpp"

namespace page { namespace phys
{
	namespace
	{
		/**
		 * The amount of time it takes for a controller to fade it.
		 */
		const float fadeInDuration = .25;

		/**
		 * The amount of time it takes for a controller to fade out.
		 */
		const float fadeOutDuration = .25;

		/**
		 * The exponent for the HermiteScale function applied to the opacity.
		 */
		const float fadeExponent = 1.5;
	}

	/*-------------+
	| constructors |
	+-------------*/

	Controller::Controller(AnimationLayer layer) :
		layer(layer), alive(true), opacity(0) {}

	/*----------+
	| observers |
	+----------*/

	bool Controller::IsAlive() const
	{
		return alive || opacity;
	}

	float Controller::GetOpacity() const
	{
		return math::HermiteScale(opacity, fadeExponent);
	}

	AnimationLayer Controller::GetLayer() const
	{
		return layer;
	}

	const Controller::Dependencies &Controller::GetDependencies() const
	{
		return dependencies;
	}

	/*----------+
	| modifiers |
	+----------*/

	void Controller::Kill()
	{
		alive = false;
	}

	void Controller::SkipFade()
	{
		opacity = alive;
	}

	/*-------+
	| update |
	+-------*/

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

	/*---------------+
	| implementation |
	+---------------*/

	void Controller::DoUpdate(float) {}

	/*-----------+
	| comparison |
	+-----------*/

	bool operator <(const Controller &c1, const Controller &c2)
	{
		return c1.GetLayer() < c2.GetLayer();
	}

	bool operator <(const Controller &c, AnimationLayer layer)
	{
		return c.GetLayer() < layer;
	}
}}
