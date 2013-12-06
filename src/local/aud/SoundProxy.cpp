#include <cassert>

#include "Sound.hpp"
#include "SoundProxy.hpp"

namespace page { namespace aud
{
	/*-------------+
	| constructors |
	+-------------*/

	SoundProxy::SoundProxy() {}

	SoundProxy::SoundProxy(const std::shared_ptr<Sound> &sound) :
		sound(sound) {}

	/*----------+
	| observers |
	+----------*/

	bool SoundProxy::IsPlaying() const
	{
		assert(sound);
		return sound->IsPlaying();
	}

	float SoundProxy::GetVolume() const
	{
		assert(sound);
		return sound->GetVolume();
	}

	SoundProxy::operator bool() const
	{
		return sound != nullptr;
	}

	bool SoundProxy::operator ==(const SoundProxy &other) const
	{
		return sound == other.sound;
	}

	bool SoundProxy::operator !=(const SoundProxy &other) const
	{
		return sound != other.sound;
	}

	/*----------+
	| modifiers |
	+----------*/

	void SoundProxy::Reset()
	{
		sound.reset();
	}

	void SoundProxy::Reset(const std::shared_ptr<Sound> &sound)
	{
		this->sound = sound;
	}

	void SoundProxy::Stop()
	{
		assert(sound);
		sound->Stop();
	}

	void SoundProxy::Pause()
	{
		assert(sound);
		sound->Pause();
	}

	void SoundProxy::Resume()
	{
		assert(sound);
		sound->Resume();
	}

	void SoundProxy::SetVolume(float volume)
	{
		assert(sound);
		sound->SetVolume(volume);
	}
}}
