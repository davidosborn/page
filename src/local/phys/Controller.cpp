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
