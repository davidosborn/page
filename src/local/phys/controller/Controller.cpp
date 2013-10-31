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

	auto Controller::GetDependencies() const -> Dependencies
	{
		return DoGetDependencies();
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

	auto Controller::DoGetDependencies() const -> Dependencies
	{
		return Dependencies();
	}

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
