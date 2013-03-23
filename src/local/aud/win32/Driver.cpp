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

#include <climits> // CHAR_BIT

#include "../../err/Exception.hpp"
#include "../../wnd/win32/Window.hpp" // Window->wnd::Window
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

	// factory function
	Driver *MakeDriver(wnd::Window &wnd)
	{
		return new win32::Driver(dynamic_cast<wnd::win32::Window &>(wnd));
	}
}}
