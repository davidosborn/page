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
