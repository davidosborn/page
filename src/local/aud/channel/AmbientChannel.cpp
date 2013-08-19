#include <algorithm> // max, min

#include "../../math/float.hpp" // DecibelToLinear
#include "../../math/interp.hpp" // HermiteScale
#include "../Sound.hpp" // Sound::{Get{FadeDuration,Level,Volume},IsStopping}
#include "AmbientChannel.hpp"

namespace page { namespace aud
{
	// construct
	AmbientChannel::AmbientChannel(const Sound &sound) :
		Channel(sound.GetPlayPosition()), playing(!sound.IsStopping()),
		fadeDuration(sound.GetFadeDuration()), level(sound.GetLevel()),
		volume(sound.GetVolume()) {}

	// state
	bool AmbientChannel::IsAlive() const
	{
		return playing || level;
	}

	// update
	void AmbientChannel::DoUpdate(float deltaTime)
	{
		level = playing ?
			std::min(level + deltaTime / fadeDuration, 1.f) :
			std::max(level - deltaTime / fadeDuration, 0.f);
	}

	// modifiers
	void AmbientChannel::Stop()
	{
		playing = false;
		if (!fadeDuration) level = 0;
	}
	void AmbientChannel::Stop(float fadeDuration)
	{
		playing = false;
		if (!fadeDuration) level = 0;
		else this->fadeDuration = fadeDuration;
	}
	void AmbientChannel::SetVolume(float volume)
	{
		this->volume = volume;
	}

	// state
	float AmbientChannel::DoGetLinearVolume() const
	{
		return math::DecibelToLinear(volume) *
			math::HermiteScale(level, fadeExponent);
	}
}}
