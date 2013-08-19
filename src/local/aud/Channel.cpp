#include <algorithm> // min

#include "../math/float.hpp" // LinearToDecibel
#include "../math/interp.hpp" // HermiteScale
#include "Channel.hpp"

namespace page { namespace aud
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
}}
