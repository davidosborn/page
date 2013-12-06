#include <iostream> // cout
#include <string>

#include <AL/al.h>

#include "../../err/Exception.hpp"
#include "../../log/Indenter.hpp"
#include "../../math/float.hpp" // DecibelToLinear
#include "../../util/cpp.hpp" // STRINGIZE
#include "../../util/string/operations.hpp" // Trim
#include "../../wnd/Window.hpp" // REGISTER_DRIVER
#include "../DriverRegistry.hpp" // REGISTER_DRIVER
#include "channel/AmbientChannel.hpp" // AmbientChannel::AmbientChannel
#include "channel/SpatialChannel.hpp" // SpatialChannel::SpatialChannel
#include "Driver.hpp"

namespace page { namespace aud { namespace openal
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

	/*-------------+
	| registration |
	+-------------*/

	REGISTER_DRIVER(Driver, wnd::Window, STRINGIZE(OPENAL_NAME) " audio driver", 25)
}}}
