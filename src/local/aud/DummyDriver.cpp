/**
 * @copyright
 *
 * Copyright (c) 2006-2014 David Osborn
 *
 * Permission is granted to use and redistribute this software in source and
 * binary form, with or without modification, subject to the following
 * conditions:
 *
 * 1. Redistributions in source form must retain the above copyright notice,
 *    this list of conditions, and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions, and the following disclaimer in the same place
 *    and form as other copyright, license, and disclaimer information.
 *
 * 3. Redistributions in binary form must also include an acknowledgement in the
 *    same place and form as other acknowledgements (such as the credits),
 *    similar in substance to the following:
 *
 *       Portions of this software are based on the work of David Osborn.
 *
 * This software is provided "as is", without any express or implied warranty.
 * In no event will the authors be liable for any damages arising out of the use
 * of this software.
 */

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
