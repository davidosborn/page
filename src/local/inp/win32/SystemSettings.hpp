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
