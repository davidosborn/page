#include <cassert>

#include "DriverRegistry.hpp" // REGISTER_DRIVER
#include "NullDriver.hpp"

namespace page { namespace aud
{
	// construct
	NullDriver::NullDriver(wnd::Window &wnd) : Driver(wnd)
	{
		Init();
	}

	// modifiers
	void NullDriver::SetVolume(float) {}

	// limits
	unsigned NullDriver::MaxAmbientChannels() const
	{
		return 0;
	}
	unsigned NullDriver::MaxSpatialChannels() const
	{
		return 0;
	}
	unsigned NullDriver::MaxPersistentSpatialChannels() const
	{
		return 0;
	}

	// channel factory functions
	AmbientChannel *NullDriver::MakeAmbientChannel(const Sound &) const
	{
		assert(!"not supported");
		return 0;
	}
	SpatialChannel *NullDriver::MakeSpatialChannel(const phys::Sound &) const
	{
		assert(!"not supported");
		return 0;
	}

	/*-------------+
	| registration |
	+-------------*/

	REGISTER_DRIVER(NullDriver, wnd::Window, "null audio driver", -100)
}}
