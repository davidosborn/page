#ifndef    page_local_inp_Driver_hpp
#   define page_local_inp_Driver_hpp

#	include <memory> // shared_ptr
#	include <vector>

#	include <boost/signal.hpp>

#	include "../cache/proxy/Proxy.hpp"
#	include "../math/fwd.hpp" // Euler
#	include "../util/class/special_member_functions.hpp" // Polymorphic, Uncopyable
#	include "Button.hpp"
#	include "Key.hpp"
#	include "PollState.hpp"

namespace page
{
	namespace gui { class Root; }
	namespace res { class Cursor; }
	namespace wnd { class Window; }
}

namespace page { namespace inp
{
	class Device;

	/**
	 * The base class for input drivers.
	 *
	 * @note Uses the "Non-Virtual Interface" pattern.
	 */
	class Driver :
		public util::Polymorphic<Driver>,
		public util::Uncopyable<Driver>
	{
		/*-------------+
		| constructors |
		+-------------*/

		public:
		explicit Driver(wnd::Window &);

		/*------------+
		| cursor mode |
		+------------*/

		/**
		 * The available modes for mouse-input interpretation and cursor
		 * representation.
		 */
		enum class CursorMode
		{
			/**
			 * The cursor will not be displayed and mouse input will be ignored.
			 */
			none,

			/**
			 * The cursor will be displayed and used for pointing.
			 */
			point,

			/**
			 * The cursor will not be displayed and mouse input will be used for
			 * mouse looking.
			 */
			look
		};

		/**
		 * Returns the current cursor mode.
		 */
		CursorMode GetCursorMode() const;

		/**
		 * Sets the cursor mode.
		 */
		void SetCursorMode(CursorMode);

		/*-----------------------------------+
		| cursor state for CursorMode::point |
		+-----------------------------------*/

		/**
		 * Returns the cursor's position.
		 *
		 * @note For use with CursorMode::point.
		 */
		const math::Vec2 &GetCursorPosition() const;

		/**
		 * Returns true if the cursor is involved in a drag operation.
		 *
		 * @note For use with CursorMode::point.
		 */
		bool IsDragging() const;

		/**
		 * Returns the origin of the current drag operation.
		 *
		 * @note For use with CursorMode::point.
		 */
		const math::Vec2 &GetDragOrigin() const;

		/**
		 * Returns the resource to be used to style the cursor.
		 *
		 * @note For use with CursorMode::point.
		 */
		cache::Proxy<res::Cursor> GetCursor() const;

		/**
		 * Sets the resource to be used to style the cursor.
		 *
		 * @note For use with CursorMode::point.
		 */
		void SetCursor(const cache::Proxy<res::Cursor> &);

		/*----------------------------------+
		| cursor state for CursorMode::look |
		+----------------------------------*/

		/**
		 * Returns the cumulative rotation.
		 *
		 * @note For use with CursorMode::look.
		 */
		const math::Euler<> &GetRotation() const;

		/**
		 * Returns the cumulative lift.
		 *
		 * @note For use with CursorMode::look.
		 */
		float GetLift() const;

		/**
		 * Returns the cumulative zoom.
		 *
		 * @note For use with CursorMode::look.
		 */
		float GetZoom() const;

		/*--------------+
		| control state |
		+--------------*/

		/**
		 * Returns the state of the directional control.
		 */
		const math::Vec2 &GetDirection() const;

		/**
		 * Returns the state of the specified modifier.
		 */
		bool GetModifier(Modifier::Type) const;

		/*---------------+
		| cursor signals |
		+---------------*/

		boost::signal<void (const math::Vec2 &position, Button, bool _double)> downSig;
		boost::signal<void (const math::Vec2 &position, Button, bool _double)> clickSig;
		boost::signal<void (const math::Vec2 &origin, Button, bool _double)> dragSig;
		boost::signal<void (const math::Vec2 &origin, const math::Vec2 &position, Button, bool _double)> dropSig;
		boost::signal<void (const math::Vec2 &origin, const math::Vec2 &position, Button, bool _double)> cancelDragSig;
		boost::signal<void (const math::Vec2 &position, float scroll)> scrollSig;

		/*----------------+
		| control signals |
		+----------------*/

		boost::signal<void (Key)> keySig;
		boost::signal<void (char)> charSig;

		/*-------+
		| update |
		+-------*/

		void Update();

		/*--------------+
		| window access |
		+--------------*/

		wnd::Window &GetWindow();
		const wnd::Window &GetWindow() const;

		/*---------------+
		| implementation |
		+---------------*/

		private:
		/**
		 * @copydoc SetCursorMode.
		 */
		virtual void DoSetCursorMode(CursorMode) = 0;

		/**
		 * @copydoc SetCursor().
		 */
		virtual void DoSetCursor(const cache::Proxy<res::Cursor> &) = 0;

		/**
		 * Returns the current state of the driver.
		 */
		virtual PollState Poll() const = 0;

		/**
		 * Returns the cursor's position in pixels.
		 */
		virtual math::Vec2u GetRawCursorPosition() const = 0;

		/*-----------------------+
		| cursor signal handlers |
		+-----------------------*/

		void OnDown(const math::Vec2 &, Button, bool);
		void OnClick(const math::Vec2 &, Button, bool);
		void OnDrag(const math::Vec2 &, Button, bool);
		void OnDrop(const math::Vec2 &, const math::Vec2 &, Button, bool);
		void OnCancelDrag(const math::Vec2 &, const math::Vec2 &, Button, bool);
		void OnScroll(const math::Vec2 &, float);

		/*------------------------+
		| control signal handlers |
		+------------------------*/

		void OnKey(Key);
		void OnChar(char);

		/*-------------+
		| data members |
		+-------------*/

		/**
		 * The parent window.
		 */
		wnd::Window &window;

		/**
		 * The input devices available to the driver.
		 */
		std::vector<std::shared_ptr<Device>> devices;

		/**
		 * The current cursor mode.
		 */
		CursorMode cursorMode = CursorMode::point;

		/**
		 * The resource used to style the cursor.
		 */
		cache::Proxy<res::Cursor> cursor;

		/**
		 * The cursor state for Driver::CursorMode::point.
		 */
		struct CursorPointState
		{
			// pointing
			bool inRange = false;
			math::Vec2 position;

			// dragging
			bool dragging = false;
			math::Vec2 dragOrigin;
		} cursorPointState;

		/**
		 * The current cursor state for CursorMode::look.
		 */
		CursorLookState cursorLookState;

		/**
		 * The current control state.
		 */
		ControlState controlState;
	};
}}

#endif
