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

#include "../cfg/vars.hpp"
#include "channel/AmbientChannel.hpp"
#include "channel/SpatialChannel.hpp"
#include "Driver.hpp"
#include "Sound.hpp"

namespace page { namespace aud
{
	// construct/destroy
	Driver::Driver(wnd::Window &window) : window(window), scene(0) {}
	Driver::~Driver() {}

	// update
	void Driver::Update(float deltaTime)
	{
		// update ambient sounds
		for (Sounds::iterator iter(sounds.begin()); iter != sounds.end();)
		{
			Sound &sound(**iter);
			sound.Update(deltaTime);
			// remove dead sounds
			if (!sound.IsAlive() && iter->unique())
			{
				iter = sounds.erase(iter);
				continue;
			}
			if (sound.IsPlaying())
			{
				// assign channel if none already assigned
				// NOTE: only used to resume from paused state
				if (!sound.HasChannel() &&
					ambientChannels.size() < MaxAmbientChannels())
				{
					std::shared_ptr<AmbientChannel> channel(MakeAmbientChannel(sound));
					ambientChannels.push_back(channel);
					sound.Bind(*channel);
				}
			}
			else
			{
				// release channel if not in use
				if (sound.HasChannel())
					sound.ReleaseChannel();
			}
			++iter;
		}
		// update ambient channels
		for (AmbientChannels::iterator iter(ambientChannels.begin()); iter != ambientChannels.end();)
		{
			AmbientChannel &channel(**iter);
			channel.Update(deltaTime);
			// remove dead channels
			if (!channel.IsAlive())
			{
				iter = ambientChannels.erase(iter);
				continue;
			}
			++iter;
		}
		// update spacial channels
		if (MaxPersistentSpatialChannels())
		{
			// update channels to represent closest sources
			// FIXME: build list of n closest phys::Sounds
			// FIXME: ensure sceneChannels is partitioned with dead channels
			// on one side and living channels on the other, the more dead
			// the further from the center
			// FIXME: for each living channel, try to match it with a
			// phys::Sound; if there's no match, kill the sound; if the
			// ID is not invalid, meaning the phys::Sound still exists, but
			// is out of range, fade out the channel
			// FIXME: for each phys::Sound, try to match it to a living
			// channel; if there's no match, try to match it with a dying
			// channel; if there's no match, replace the deadest channel
			// with a new channel representing the phys::Sound; if the
			// phys::Sound has elapsed at all, fade in the new channel
			// FIXME: update state of all channels, whether alive or dying,
			// to match phys::Sound, including volume and paused state, and
			// ensure that the playing position is reasonably close
			for (SpatialChannels::iterator iter(spacialChannels.begin()); iter != spacialChannels.end(); ++iter)
			{
				SpatialChannel &channel(**iter);
				channel.Update(deltaTime);
				if (!channel.IsAudible()) iter = spacialChannels.erase(iter);
			}
		}
	}

	// modifiers
	SoundProxy Driver::Play(const cache::Proxy<res::Sound> &res, bool loop, bool fade, float fadeDuration)
	{
		std::shared_ptr<Sound> sound(new Sound(res, loop, fade, fadeDuration));
		*sound->GetSound(); // dereference sound resource to ensure it loaded properly
		sounds.push_back(sound);
		// assign channel to sound, if available
		if (ambientChannels.size() < MaxAmbientChannels())
		{
			std::shared_ptr<AmbientChannel> channel(MakeAmbientChannel(*sound));
			ambientChannels.push_back(channel);
			sound->Bind(*channel);
		}
		return SoundProxy(sound);
	}
	void Driver::Stop()
	{
		// NOTE: this function only applies to ambient sounds and channels
		for (AmbientChannels::iterator iter(ambientChannels.begin()); iter != ambientChannels.end(); ++iter)
		{
			AmbientChannel &channel(**iter);
			channel.Stop(false);
		}
	}
	void Driver::Pause()
	{
		// FIXME: we should simply erase all channels; this would require us
		// to unbind the ambientChannels from the sounds
		for (AmbientChannels::iterator iter(ambientChannels.begin()); iter != ambientChannels.end(); ++iter)
		{
			AmbientChannel &channel(**iter);
			// FIXME: waiting for implementation
			//channel.Pause();
		}
		for (SpatialChannels::iterator iter(spacialChannels.begin()); iter != spacialChannels.end(); ++iter)
		{
			SpatialChannel &channel(**iter);
			// FIXME: waiting for implementation
			//channel.Pause();
		}
	}
	void Driver::Resume()
	{
		// FIXME: we shouldn't have to do anything here
		for (AmbientChannels::iterator iter(ambientChannels.begin()); iter != ambientChannels.end(); ++iter)
		{
			AmbientChannel &channel(**iter);
			// FIXME: waiting for implementation
			//channel.Resume();
		}
		for (SpatialChannels::iterator iter(spacialChannels.begin()); iter != spacialChannels.end(); ++iter)
		{
			SpatialChannel &channel(**iter);
			// FIXME: waiting for implementation
			//channel.Resume();
		}
	}

	// window access
	wnd::Window &Driver::GetWindow()
	{
		return window;
	}
	const wnd::Window &Driver::GetWindow() const
	{
		return window;
	}

	// inspiration modifiers
	void Driver::Imbue(const phys::Scene *scene)
	{
		this->scene = scene;
	}

	// deferred initialization
	void Driver::Init()
	{
		SetVolume(*CVAR(audioVolume));
	}

	// inspiration access
	const phys::Scene *Driver::GetScene() const
	{
		return scene;
	}
}}
