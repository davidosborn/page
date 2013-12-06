#include <climits> // CHAR_BIT

#include "../../err/Exception.hpp"
#include "../../util/cpp.hpp" // STRINGIZE
#include "../../wnd/win32/Window.hpp" // REGISTER_DRIVER, Window->wnd::Window
#include "../DriverRegistry.hpp" // REGISTER_DRIVER
#include "Driver.hpp"

namespace page { namespace aud
{
	namespace win32
	{
		const unsigned
			channels      = 2,
			bitsPerSample = 16,
			sampleRate    = 22050;

		// construct/destroy
		Driver::Driver(wnd::win32::Window &wnd) : aud::Driver(wnd)
		{
			PCMWAVEFORMAT wf =
			{
				WAVE_FORMAT_PCM,
				channels,
				sampleRate,
				channels * bitsPerSample / CHAR_BIT * sampleRate,
				channels * bitsPerSample / CHAR_BIT,
				bitsPerSample
			};
			if (waveOutOpen(&wo, WAVE_MAPPER,
				reinterpret_cast<LPWAVEFORMATEX>(&wf),
				reinterpret_cast<DWORD_PTR>(WaveOutProc),
				reinterpret_cast<DWORD_PTR>(this), CALLBACK_FUNCTION) != MMSYSERR_NOERROR)
					THROW((err::Exception<err::AudModuleTag, err::Win32PlatformTag>("failed to open audio output device") <<
						boost::errinfo_api_function("waveOutOpen")))

			Init();
		}
		Driver::~Driver()
		{
			waveOutClose(wo);
		}

		// update
		void Driver::Update(float deltaTime)
		{
			// FIXME: implement
		}

		// modifiers
		void Driver::SetVolume(float volume)
		{
			// FIXME: implement
		}

		// limits
		unsigned Driver::MaxAmbientChannels() const
		{
			return 1;
		}
		unsigned Driver::MaxSpatialChannels() const
		{
			return 0;
		}
		unsigned Driver::MaxPersistentSpatialChannels() const
		{
			return 0;
		}

		// channel factory functions
		AmbientChannel *Driver::MakeAmbientChannel(const Sound &) const
		{
			// FIXME: implement
			return 0;
		}
		SpatialChannel *Driver::MakeSpatialChannel(const phys::Sound &) const
		{
			// FIXME: implement
			return 0;
		}

		// callback
		void CALLBACK Driver::WaveOutProc(HWAVEOUT wo, UINT msg, DWORD_PTR instance, DWORD_PTR, DWORD_PTR)
		{
			Driver &driver(*reinterpret_cast<Driver *>(instance));
			// FIXME: implement
		}
	}

	/*-------------+
	| registration |
	+-------------*/

	REGISTER_DRIVER(Driver, wnd::win32::Window, STRINGIZE(WIN32_NAME) " audio driver")
}}
