#ifndef    page_local_aud_openal_Driver_hpp
#   define page_local_aud_openal_Driver_hpp

#	include <AL/alc.h> // ALC{context,device}

#	include "../Driver.hpp"

namespace page { namespace aud { namespace openal
{
	struct Driver : aud::Driver
	{
		// construct/destroy
		explicit Driver(wnd::Window &);
		~Driver();

		// update
		void Update(float deltaTime);

		// modifiers
		void Play(const cache::Proxy<res::Sound> &, bool loop, bool fade);
		void Stop();
		void Pause();
		void Resume();
		void SetVolume(float);

		private:
		// limits
		unsigned MaxAmbientChannels() const;
		unsigned MaxSpatialChannels() const;
		unsigned MaxPersistentSpatialChannels() const;

		// channel factory functions
		AmbientChannel *MakeAmbientChannel(const Sound &) const;
		SpatialChannel *MakeSpatialChannel(const phys::Sound &) const;

		ALCdevice *device;
		ALCcontext *context;
	};
}}}

#endif
