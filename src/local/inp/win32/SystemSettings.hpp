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

#ifndef    page_local_inp_win32_SystemSettings_hpp
#   define page_local_inp_win32_SystemSettings_hpp

#	include "../../util/class/Monostate.hpp"

namespace page { namespace inp { namespace win32
{
	/**
	 * System settings that are used by the Win32 input driver and associated
	 * devices.
	 */
	class SystemSettings : public util::Monostate<SystemSettings>
	{
		/*-------------+
		| constructors |
		+-------------*/

		public:
		SystemSettings();

		/*----------+
		| observers |
		+----------*/

		/**
		 * @return doubleTime.
		 */
		UINT GetDoubleTime() const;

		/**
		 * @return doubleThreshold.
		 */
		math::Vec2u GetDoubleThreshold() const;

		/**
		 * @return dragThreshold.
		 */
		math::Vec2u GetDragThreshold() const;

		/**
		 * @return repeatDelay.
		 */
		unsigned GetRepeatDelay() const;

		/**
		 * @return repeatSpeed.
		 */
		DWORD GetRepeatSpeed() const;

		/*----------+
		| modifiers |
		+----------*/

		/**
		 * Updates the stored settings to reflect changes in the system.
		 *
		 * @note This function should be called in response to a
		 * @c WM_SETTINGCHANGE message.
		 */
		void Update();

		/*-------------+
		| data members |
		+-------------*/

		private:
		/**
		 * The value returned by GetDoubleClickTime().
		 */
		UINT doubleTime;

		/**
		 * A vector containing the maximum distance the mouse can move between
		 * clicks before it will no longer be seen as a double click.
		 *
		 * @sa GetSystemMetrics
		 * @sa SM_CXDOUBLECLK
		 * @sa SM_CYDOUBLECLK
		 */
		math::Vec2u doubleThreshold;

		/**
		 * A vector containing the minimum distance the mouse needs to move
		 * while a mouse button is held down for it to become a drag operation.
		 *
		 * @sa GetSystemMetrics
		 * @sa SM_CXDRAG
		 * @sa SM_CYDRAG
		 */
		math::Vec2u dragThreshold;

		/**
		 * The delay from when a key is first pressed to when it starts
		 * repeating if it is held down.
		 *
		 * @sa SystemParametersInfo
		 * @sa SPI_GETKEYBOARDDELAY
		 */
		unsigned repeatDelay;

		/**
		 * The interval at which a key will repeat if it is held down.
		 *
		 * @sa SystemParametersInfo
		 * @sa SPI_GETKEYBOARDSPEED
		 */
		DWORD repeatSpeed;
	};
}}}

#endif
