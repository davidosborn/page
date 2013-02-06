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

#ifndef    page_local_aud_Driver_hpp
#   define page_local_aud_Driver_hpp

#	include <memory> // shared_ptr
#	include <vector>
#	include "../cache/fwd.hpp" // Proxy
#	include "../util/NonCopyable.hpp"

namespace page
{
	namespace env { class Window; }
	namespace phys
	{
		class Scene;
		class Sound;
	}
	namespace res { class Sound; }

	namespace aud
	{
		class AmbientChannel;
		class Sound;
		class SpatialChannel;

		struct SoundProxy
		{
			// construct
			SoundProxy();
			explicit SoundProxy(const std::shared_ptr<Sound> &);

			// state
			bool IsPlaying() const;
			float GetVolume() const;

			// modifiers
			void Reset();
			void Reset(const std::shared_ptr<Sound> &);
			void Stop();
			void Pause();
			void Resume();
			void SetVolume(float);

			// validity
			operator bool() const;

			// relational operators
			bool operator ==(const SoundProxy &) const;
			bool operator !=(const SoundProxy &) const;

			private:
			std::shared_ptr<Sound> sound;
		};

		struct Driver : util::NonCopyable
		{
			// construct/destroy
			explicit Driver(env::Window &);
			virtual ~Driver();

			// update
			void Update(float deltaTime);

			// modifiers
			SoundProxy Play(const cache::Proxy<res::Sound> &,
				bool loop = false, bool fade = false, float fadeDuration = 1);
			void Stop();
			void Pause();
			void Resume();
			virtual void SetVolume(float) = 0;

			// window access
			env::Window &GetWindow();
			const env::Window &GetWindow() const;

			// inspiration modifiers
			void Imbue(const phys::Scene *);

			protected:
			// deferred initialization
			// must be called at end of derived constructor
			void Init();

			// inspiration access
			const phys::Scene *GetScene() const;

			private:
			// limits
			virtual unsigned MaxAmbientChannels() const = 0;
			virtual unsigned MaxSpatialChannels() const = 0;
			virtual unsigned MaxPersistentSpatialChannels() const = 0;

			// channel factory functions
			virtual AmbientChannel *MakeAmbientChannel(const Sound &) const = 0;
			virtual SpatialChannel *MakeSpatialChannel(const phys::Sound &) const = 0;

			env::Window &wnd;
			typedef std::vector<std::shared_ptr<Sound>> Sounds;
			Sounds sounds;
			typedef std::vector<std::shared_ptr<AmbientChannel>> AmbientChannels;
			AmbientChannels ambientChannels;
			typedef std::vector<std::shared_ptr<SpatialChannel>> SpatialChannels;
			SpatialChannels spacialChannels;
			const phys::Scene *scene;
		};

#	ifdef HAVE_AUDIO
		// factory function
		Driver *MakeDriver(env::Window &);
#	endif
	}
}

#endif
