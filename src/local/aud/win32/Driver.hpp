#ifndef    page_local_aud_win32_Driver_hpp
#   define page_local_aud_win32_Driver_hpp

#	include <windows.h> // CALLBACK, DWORD_PTR, mmsystem.h, UINT
#	include <mmsystem.h> // HWAVEOUT

#	include "../Driver.hpp"

namespace page { namespace wnd { namespace win32 { class Window; }}}

namespace page { namespace aud { namespace win32
{
	struct Driver : aud::Driver
	{
		// construct/destroy
		explicit Driver(wnd::win32::Window &);
		~Driver();

		// update
		void Update(float deltaTime);

		// modifiers
		void SetVolume(float);

		private:
		// limits
		unsigned MaxAmbientChannels() const;
		unsigned MaxSpatialChannels() const;
		unsigned MaxPersistentSpatialChannels() const;

		// channel factory functions
		AmbientChannel *MakeAmbientChannel(const Sound &) const;
		SpatialChannel *MakeSpatialChannel(const phys::Sound &) const;

		// callback
		static void CALLBACK WaveOutProc(HWAVEOUT, UINT, DWORD_PTR, DWORD_PTR, DWORD_PTR);

		HWAVEOUT wo;
	};
}}}

#endif
