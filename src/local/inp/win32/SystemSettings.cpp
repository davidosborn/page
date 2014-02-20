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

#include "SystemSettings.hpp"

namespace page { namespace inp { namespace win32
{
	/*-------------+
	| constructors |
	+-------------*/

	SystemSettings::SystemSettings()
	{
		Update();
	}

	/*----------+
	| observers |
	+----------*/

	UINT SystemSettings::GetDoubleTime() const
	{
		return doubleTime;
	}

	math::Vec2u SystemSettings::GetDoubleThreshold() const
	{
		return doubleThreshold;
	}

	math::Vec2u SystemSettings::GetDragThreshold() const
	{
		return dragThreshold;
	}

	unsigned SystemSettings::GetRepeatDelay() const
	{
		return repeatDelay;
	}

	DWORD SystemSettings::GetRepeatSpeed() const
	{
		return repeatSpeed;
	}

	/*----------+
	| modifiers |
	+----------*/

	void SystemSettings::Update()
	{
		doubleTime = GetDoubleClickTime();
		doubleRange = math::Vec2i(
			GetSystemMetrics(SM_CXDOUBLECLK),
			GetSystemMetrics(SM_CYDOUBLECLK)) / 2;
		dragRange = math::Vec2i(
			GetSystemMetrics(SM_CXDRAG),
			GetSystemMetrics(SM_CYDRAG)) / 2;
		if (!SystemParametersInfo(SPI_GETKEYBOARDDELAY, 0, &repeatDelay, 0))
			THROW((err::Exception<err::InpModuleTag, err::Win32PlatformTag>("failed to query keyboard repeat delay") <<
				boost::errinfo_api_function("SystemParametersInfo")))
		repeatDelay = (repeatDelay + 1) * 250;
		if (!SystemParametersInfo(SPI_GETKEYBOARDSPEED, 0, &repeatSpeed, 0))
			THROW((err::Exception<err::InpModuleTag, err::Win32PlatformTag>("failed to query keyboard repeat speed") <<
				boost::errinfo_api_function("SystemParametersInfo")))
		repeatSpeed = 400 - repeatSpeed * 12;
	}
}}}
