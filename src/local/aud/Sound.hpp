/**
 * @section license
 *
 * Copyright (c) 2006-2012 David Osborn
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

#ifndef    page_local_aud_Sound_hpp
#   define page_local_aud_Sound_hpp

#	include "../cache/fwd.hpp" // Proxy
#	include "../util/raii/copy_ptr.hpp"

namespace page { namespace res { class Sound; }}

namespace page { namespace aud
{
	class AmbientChannel;

	struct Sound
	{
		// construct
		explicit Sound(const cache::Proxy<res::Sound> &, bool loop,
			bool fade, float fadeDuration);

		// attributes
		const cache::Proxy<res::Sound> &GetSound() const;

		// state
		bool IsAlive() const;
		bool IsPlaying() const;
		bool IsStopping() const;
		bool IsLooping() const;
		float GetFadeDuration() const;
		float GetLevel() const;
		float GetPlayPosition() const;
		float GetVolume() const;

		// update
		void Update(float deltaTime);

		// modifiers
		void Stop();
		void Stop(bool fade);
		void Stop(bool fade, float fadeDuration);
		void Pause();
		void Resume();
		void SetVolume(float);

		// channel binding
		// state changes will propogate to bound channel
		bool HasChannel() const;
		AmbientChannel &GetChannel() const;
		void Bind(AmbientChannel &);
		void ReleaseChannel();

		private:
		util::copy_ptr<cache::Proxy<res::Sound>> sound;
		bool playing, paused, loop, fade;
		float fadeDuration, level, playPosition, volume;
		AmbientChannel *channel;
	};
}}

#endif
