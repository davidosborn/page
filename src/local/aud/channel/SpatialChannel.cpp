#include <algorithm> // max

#include "../../math/float.hpp" // DecibelToLinear, Inf
#include "../../math/interp.hpp" // HermiteScale
#include "../../phys/node/Sound.hpp" // Sound->util::Identifiable, Sound::Get{PlayPosition,Volume}
#include "SpatialChannel.hpp"

namespace page { namespace aud
{
	float fadeOutDuration = 1;

	// construct
	SpatialChannel::SpatialChannel(const phys::Sound &sound) :
		Channel(sound.GetPlayPosition()), id(sound.GetId()),
		occluded(false), level(1) {}

	// state
	bool SpatialChannel::IsAudible() const
	{
		return level;
	}

	// update
	void SpatialChannel::DoUpdate(float deltaTime)
	{
		if (occluded)
			level = std::max(level - deltaTime / fadeOutDuration, 0.f);
	}

	// modifiers
	void SpatialChannel::Occlude()
	{
		occluded = true;
	}

	// sound access
	phys::Sound *SpatialChannel::GetSound() const
	{
		return util::GetPointerById<phys::Sound>(id);
	}
	util::Identifiable::Id SpatialChannel::GetId() const
	{
		return id;
	}

	// state
	float SpatialChannel::DoGetLinearVolume() const
	{
		phys::Sound *sound = GetSound();
		return sound ?
			math::DecibelToLinear(sound->GetVolume()) *
			math::HermiteScale(level, fadeExponent) : -math::Inf();
	}
}}
