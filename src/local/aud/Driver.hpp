#ifndef    page_local_aud_Driver_hpp
#   define page_local_aud_Driver_hpp

#	include <memory> // shared_ptr
#	include <vector>

#	include "../cache/proxy/Proxy.hpp"
#	include "../util/class/special_member_functions.hpp" // Uncopyable

namespace page
{
	namespace phys
	{
		class Scene;
		class Sound;
	}
	namespace res { class Sound; }
	namespace wnd { class Window; }
}

namespace page { namespace aud
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
		explicit operator bool() const;

		// relational operators
		bool operator ==(const SoundProxy &) const;
		bool operator !=(const SoundProxy &) const;

		private:
		std::shared_ptr<Sound> sound;
	};

	struct Driver : util::Uncopyable<Driver>
	{
		// construct/destroy
		explicit Driver(wnd::Window &);
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
		wnd::Window &GetWindow();
		const wnd::Window &GetWindow() const;

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

		wnd::Window &wnd;
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
	Driver *MakeDriver(wnd::Window &);
#	endif
}}

#endif
