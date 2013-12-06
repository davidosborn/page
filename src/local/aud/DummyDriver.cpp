#include <cassert>

#include "../wnd/Window.hpp" // REGISTER_DRIVER
#include "DriverRegistry.hpp" // REGISTER_DRIVER
#include "DummyDriver.hpp"

namespace page { namespace aud
{
	/*-------------+
	| constructors |
	+-------------*/

	DummyDriver::DummyDriver(wnd::Window &wnd) : Driver(wnd)
	{
		Init();
	}

	/*----------------------+
	| Driver implementation |
	+----------------------*/

	void DummyDriver::SetVolume(float) {}

	unsigned DummyDriver::MaxAmbientChannels() const
	{
		return 0;
	}

	unsigned DummyDriver::MaxSpatialChannels() const
	{
		return 0;
	}

	unsigned DummyDriver::MaxPersistentSpatialChannels() const
	{
		return 0;
	}

	AmbientChannel *DummyDriver::MakeAmbientChannel(const Sound &) const
	{
		assert(!"not supported");
		return 0;
	}

	SpatialChannel *DummyDriver::MakeSpatialChannel(const phys::Sound &) const
	{
		assert(!"not supported");
		return 0;
	}

	/*-------------+
	| registration |
	+-------------*/

	REGISTER_DRIVER(DummyDriver, wnd::Window, "dummy audio driver", -100)
}}
