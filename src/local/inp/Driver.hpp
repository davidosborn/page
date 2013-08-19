#ifndef    page_local_inp_Driver_hpp
#   define page_local_inp_Driver_hpp

#	include <memory> // shared_ptr
#	include <vector>

#	include <boost/signal.hpp>

#	include "../math/fwd.hpp" // Euler
#	include "../math/Vector.hpp"
#	include "../util/class/special_member_functions.hpp" // Uncopyable
#	include "Button.hpp"
#	include "Key.hpp"
#	include "State.hpp"

namespace page
{
	namespace ui { class UserInterface; }
	namespace wnd { class Window; }

	namespace inp
	{
		class Device;

		struct Driver : util::Uncopyable<Driver>
		{
			// construct/destroy
			explicit Driver(wnd::Window &);
			virtual ~Driver();

			// cursor mode
			enum CursorMode
			{
				noCursorMode,
				pointCursorMode,
				lookCursorMode
			};
			CursorMode GetCursorMode() const;
			void SetCursorMode(CursorMode);

			// cursor pointing state
			const math::Vec2 &GetCursorPosition() const;

			// cursor dragging state
			bool IsDragging() const;
			const math::Vec2 &GetDragOrigin() const;

			// control state
			const math::Vec2 &GetDirection() const;
			enum Modifier
			{
				runModifier,
				altModifier
			};
			bool GetModifier(Modifier) const;

			// look state
			const math::Euler<> &GetRotation() const;
			float GetLift() const;
			float GetZoom() const;

			// update
			void Update();

			// window access
			wnd::Window &GetWindow();
			const wnd::Window &GetWindow() const;

			// inspiration modifiers
			void Imbue(const ui::UserInterface *);

			// pointer signals
			boost::signal<void (const math::Vec2 &position, Button, bool _double)> downSig;
			boost::signal<void (const math::Vec2 &position, Button, bool _double)> clickSig;
			boost::signal<void (const math::Vec2 &origin, Button, bool _double)> dragSig;
			boost::signal<void (const math::Vec2 &origin, const math::Vec2 &position, Button, bool _double)> dropSig;
			boost::signal<void (const math::Vec2 &origin, const math::Vec2 &position, Button, bool _double)> cancelDragSig;
			boost::signal<void (const math::Vec2 &position, float scroll)> scrollSig;

			// key signals
			boost::signal<void (Key)> keySig;
			boost::signal<void (char)> charSig;

			protected:
			// inspiration access
			const ui::UserInterface *GetUserInterface() const;

			private:
			// update
			void Update(const State &);

			// state query
			virtual State Poll() const = 0;

			// cursor mode modifiers
			virtual void DoSetCursorMode(CursorMode) = 0;

			// system cursor state
			virtual math::Vec2u GetRawCursorPosition() const = 0;

			// inspiration notification
			virtual void OnImbue(const ui::UserInterface *);

			// cursor signal handlers
			void OnDown(const math::Vec2 &, Button, bool);
			void OnClick(const math::Vec2 &, Button, bool);
			void OnDrag(const math::Vec2 &, Button, bool);
			void OnDrop(const math::Vec2 &, const math::Vec2 &, Button, bool);
			void OnCancelDrag(const math::Vec2 &, const math::Vec2 &, Button, bool);
			void OnScroll(const math::Vec2 &, float);

			// key signal handlers
			void OnKey(Key);
			void OnChar(char);

			wnd::Window &wnd;
			const ui::UserInterface *userInterface;

			// devices
			typedef std::vector<std::shared_ptr<Device>> Devices;
			Devices devices;

			// cursor mode
			CursorMode cursorMode;

			// cursor pointing state
			struct PointCursor
			{
				// construct
				PointCursor();

				// pointing
				bool inRange;
				math::Vec2 position;
				// dragging
				bool dragging;
				math::Vec2 dragOrigin;
			} pointCursor;

			// control/look state
			State::Control control;
			State::Look look;
		};

		// factory function
		Driver *MakeDriver(wnd::Window &);
	}
}

#endif
