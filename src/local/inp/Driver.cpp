#include <cassert>
#include <cmath> // abs
#include <functional> // bind
#include <iomanip> // setw
#include <iostream> // cout
#include <string>
#include <utility> // move

#include <boost/io/ios_state.hpp> // ios_all_saver

#include "../math/Aabb.hpp"
#include "../math/float.hpp" // Near
#include "../wnd/Window.hpp"
#include "Device.hpp" // Device::Update
#include "DeviceRegistry.hpp" // DeviceRegistry::MakeAll
#include "Driver.hpp"

namespace page { namespace inp
{
	/*-------------+
	| constructors |
	+-------------*/

	Driver::Driver(wnd::Window &window) : window(window)
	{
		// connect signal handlers
		{
			using std::bind;
			using namespace std::placeholders;
			downSig      .connect(bind(&Driver::OnDown,       this, _1, _2, _3));
			clickSig     .connect(bind(&Driver::OnClick,      this, _1, _2, _3));
			dragSig      .connect(bind(&Driver::OnDrag,       this, _1, _2, _3));
			dropSig      .connect(bind(&Driver::OnDrop,       this, _1, _2, _3, _4));
			cancelDragSig.connect(bind(&Driver::OnCancelDrag, this, _1, _2, _3, _4));
			scrollSig    .connect(bind(&Driver::OnScroll,     this, _1, _2));
			charSig      .connect(bind(&Driver::OnChar,       this, _1));
			keySig       .connect(bind(&Driver::OnKey,        this, _1));
		}

		// initialize devices
		for (auto &device : GLOBAL(DeviceRegistry).MakeAll(window))
			devices.push_back(std::shared_ptr<Device>(std::move(device)));
	}

	/*------------+
	| cursor mode |
	+------------*/

	Driver::CursorMode Driver::GetCursorMode() const
	{
		return cursorMode;
	}

	void Driver::SetCursorMode(CursorMode mode)
	{
		if (mode != cursorMode)
		{
			/*
			 * We call DoSetCursorMode() before changing the internal cursor
			 * mode to allow the implementation of DoSetCursorMode() to call
			 * GetCursorPosition().
			 */
			DoSetCursorMode(mode);
			cursorMode = mode;
		}
	}

	/*-----------------------------------+
	| cursor state for CursorMode::point |
	+-----------------------------------*/

	const math::Vec2 &Driver::GetCursorPosition() const
	{
		assert(cursorMode == CursorMode::point);
		return cursorPointState.position;
	}

	bool Driver::IsDragging() const
	{
		assert(cursorMode == CursorMode::point);
		return cursorPointState.dragging;
	}

	const math::Vec2 &Driver::GetDragOrigin() const
	{
		assert(cursorMode == CursorMode::point && cursorPointState.dragging);
		return cursorPointState.dragOrigin;
	}

	cache::Proxy<res::Cursor> Driver::GetCursor() const
	{
		return cursor;
	}

	void Driver::SetCursor(const cache::Proxy<res::Cursor> &cursor)
	{
		DoSetCursor(cursor);
		this->cursor = cursor;
	}

	/*----------------------------------+
	| cursor state for CursorMode::look |
	+----------------------------------*/

	const math::Euler<> &Driver::GetRotation() const
	{
		assert(cursorMode == CursorMode::look);
		return cursorLookState.rotation;
	}

	float Driver::GetLift() const
	{
		assert(cursorMode == CursorMode::look);
		return cursorLookState.lift;
	}

	float Driver::GetZoom() const
	{
		assert(cursorMode == CursorMode::look);
		return cursorLookState.zoom;
	}

	/*--------------+
	| control state |
	+--------------*/

	const math::Vec2 &Driver::GetDirection() const
	{
		return controlState.direction;
	}

	bool Driver::GetModifier(Modifier::Type modifier) const
	{
		return controlState.modifiers & modifier;
	}

	/*-------+
	| update |
	+-------*/

	void Driver::Update()
	{
		// update cursor
		if (cursorMode == CursorMode::point)
		{
			cursorPointState.position = EnterSpace(
				AabbPositionSize(
					window.GetPosition(),
					math::Vec2i(window.GetSize()) - 1),
				math::Vec2i(GetRawCursorPosition()));
			cursorPointState.inRange = Contains(math::Aabb<2>(0, 1), cursorPointState.position);
			cursorPointState.position = Min(Max(cursorPointState.position, 0), 1);
		}

		// build state by polling driver and devices
		auto pollState(Poll());
		for (auto &device : devices)
			pollState += device->Poll();

		// update internal state
		cursorLookState = pollState.look;
		controlState    = pollState.control;

		// execute events
		for (const auto &event : pollState.keyEvents)
			keySig(event);
		for (const auto &event : pollState.charEvents)
			charSig(event);
	}

	/*--------------+
	| window access |
	+--------------*/

	wnd::Window &Driver::GetWindow()
	{
		return window;
	}

	const wnd::Window &Driver::GetWindow() const
	{
		return window;
	}

	/*-----------------------+
	| cursor signal handlers |
	+-----------------------*/

	void Driver::OnDown(const math::Vec2 &pos, Button btn, bool _double)
	{
		std::cout << GetName(btn) << " mouse ";
		if (_double) std::cout << "double-";
		std::cout << "down at " << pos << std::endl;
	}

	void Driver::OnClick(const math::Vec2 &pos, Button btn, bool _double)
	{
		std::cout << GetName(btn) << " mouse ";
		if (_double) std::cout << "double-";
		std::cout << "click at " << pos << std::endl;
	}

	void Driver::OnDrag(const math::Vec2 &origin, Button btn, bool _double)
	{
		std::cout << GetName(btn) << " mouse ";
		if (_double) std::cout << "double-";
		std::cout << "dragging from " << origin << std::endl;
		cursorPointState.dragging = true;
		cursorPointState.dragOrigin = origin;
	}

	void Driver::OnDrop(const math::Vec2 &start, const math::Vec2 &pos, Button btn, bool _double)
	{
		std::cout << GetName(btn) << " mouse ";
		if (_double) std::cout << "double-";
		std::cout << "dropped at " << pos << std::endl;
		cursorPointState.dragging = false;
	}

	void Driver::OnCancelDrag(const math::Vec2 &start, const math::Vec2 &pos, Button btn, bool _double)
	{
		std::cout << GetName(btn) << " mouse ";
		if (_double) std::cout << "double-";
		std::cout << "drag cancelled at " << pos << std::endl;
		cursorPointState.dragging = false;
	}

	void Driver::OnScroll(const math::Vec2 &pos, float delta)
	{
		std::cout << "mouse scrolled " << (delta > 0 ? "foward" : "backward") << ' ';
		delta = std::abs(delta);
		if (math::Near(delta, 1.f)) std::cout << "once";
		else if (math::Near(delta, 2.f)) std::cout << "twice";
		else std::cout << delta << " times";
		std::cout << " at " << pos << std::endl;
	}

	/*------------------------+
	| control signal handlers |
	+------------------------*/

	void Driver::OnKey(Key key)
	{
		std::cout << "received " << GetName(key) << " key" << std::endl;
	}

	void Driver::OnChar(char c)
	{
		// print hexadecimal representation
		boost::io::ios_all_saver iosFormatSaver(std::cout);
		std::cout.setf(std::ios_base::hex, std::ios_base::basefield);
		std::cout.setf(std::ios_base::showbase);
		std::cout.setf(std::ios_base::internal, std::ios_base::adjustfield);
		std::cout.fill('0');
		std::cout << "character " << std::setw(4) << static_cast<unsigned>(c);
		iosFormatSaver.restore();

		// print friendly representation if available
		std::string repr(GetName(c));
		if (!repr.empty()) std::cout << " (" << repr << ')';
		std::cout << " entered" << std::endl;
	}
}}
