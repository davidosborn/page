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

#include <iostream> // cout
#include <string>

#include <AL/al.h>

#include "../../err/Exception.hpp"
#include "../../log/Indenter.hpp"
#include "../../math/float.hpp" // DecibelToLinear
#include "../../util/string/operations.hpp" // Trim
#include "channel/AmbientChannel.hpp" // AmbientChannel::AmbientChannel
#include "channel/SpatialChannel.hpp" // SpatialChannel::SpatialChannel
#include "Driver.hpp"

namespace page { namespace aud
{
	namespace openal
	{
		// construct/destroy
		Driver::Driver(wnd::Window &wnd) : aud::Driver(wnd)
		{
			// print driver information
			const char *versionPtr = alGetString(AL_VERSION);
			std::string version(versionPtr ? util::Trim(std::string(versionPtr)) : "1.0");
			std::cout << "using OpenAL driver " << version << std::endl;
			{
				log::Indenter indenter;
				if (const char *rendererPtr = alGetString(AL_RENDERER))
				{
					std::string renderer(util::Trim(std::string(rendererPtr)));
					if (!renderer.empty()) std::cout << renderer << std::endl;
				}
				if (const char *vendorPtr = alGetString(AL_VENDOR))
				{
					std::string vendor(util::Trim(std::string(vendorPtr)));
					if (!vendor.empty()) std::cout << vendor << std::endl;
				}
			}

			// open device
			if (!(device = alcOpenDevice(0)))
				THROW((err::Exception<err::AudModuleTag, err::OpenalPlatformTag>("failed to open device") <<
					boost::errinfo_api_function("alcOpenDevice")))

			// print device information
			std::string deviceName(util::Trim(std::string(
				alcGetString(device, ALC_DEVICE_SPECIFIER))));
			if (!deviceName.empty())
			{
				// HACK: should appear contiguous with driver information
				log::Indenter indenter;
				std::cout << deviceName << std::endl;
			}

			// initialize context
			if (!(context = alcCreateContext(device, 0)))
			{
				alcCloseDevice(device);
				THROW((err::Exception<err::AudModuleTag, err::OpenalPlatformTag>("failed to create context") <<
					boost::errinfo_api_function("alcCreateContext")))
			}
			if (!alcMakeContextCurrent(context))
			{
				alcDestroyContext(context);
				alcCloseDevice(device);
				THROW((err::Exception<err::AudModuleTag, err::OpenalPlatformTag>("failed to make context current") <<
					boost::errinfo_api_function("alcMakeContextCurrent")))
			}

			// perform deferred initialization
			Init();
		}
		Driver::~Driver()
		{
			alcMakeContextCurrent(0);
			alcDestroyContext(context);
			alcCloseDevice(device);
		}

		// update
		void Driver::Update(float deltaTime) {}

		// modifiers
		void Driver::SetVolume(float volume)
		{
			alListenerf(AL_GAIN, math::DecibelToLinear(volume));
		}

		// limits
		unsigned Driver::MaxAmbientChannels() const
		{
			return 8;
		}
		unsigned Driver::MaxSpatialChannels() const
		{
			return 24;
		}
		unsigned Driver::MaxPersistentSpatialChannels() const
		{
			return 16;
		}

		// channel factory functions
		AmbientChannel *Driver::MakeAmbientChannel(const Sound &sound) const
		{
			return new AmbientChannel(sound);
		}
		SpatialChannel *Driver::MakeSpatialChannel(const phys::Sound &sound) const
		{
			return new SpatialChannel(sound);
		}
	}

	// factory function
	Driver *MakeDriver(wnd::Window &wnd)
	{
		return new openal::Driver(wnd);
	}
}}
