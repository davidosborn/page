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

#include <functional> // bind
#include <iostream> // cout

#include "../aud/DriverRegistry.hpp" // DriverRegistry::Make
#include "../inp/DriverRegistry.hpp" // DriverRegistry::Make
#include "../vid/DriverRegistry.hpp" // DriverRegistry::Make
#include "Window.hpp"

namespace page { namespace wnd
{
	/*-------------+
	| constructors |
	+-------------*/

	Window::Window() : focus(false)
	{
		using namespace std::placeholders;
		exitSig .connect(std::bind(&Window::OnExit,  this));
		focusSig.connect(std::bind(&Window::OnFocus, this, _1));
		moveSig .connect(std::bind(&Window::OnMove,  this, _1));
		sizeSig .connect(std::bind(&Window::OnSize,  this, _1));
	}

	/*--------------------+
	| copy/move semantics |
	+--------------------*/

	Window::Window(const Window &other) :
		focus   (other.focus),
		position(other.position),
		size    (other.size) {}

	Window &Window::operator =(Window other)
	{
		std::swap(*this, other);
		return *this;
	}

	/*---------------+
	| initialization |
	+---------------*/

	void Window::InitState(
		bool               focus,
		math::Vec2i const& position,
		math::Vec2u const& size)
	{
		this->focus    = focus;
		this->position = position;
		this->size     = size;
	}

	void Window::ResetDrivers()
	{
		audioDriver.reset();
		inputDriver.reset();
		videoDriver.reset();
	}

	/*----------+
	| observers |
	+----------*/

	bool Window::HasFocus() const
	{
		return focus;
	}

	const math::Vec2i &Window::GetPosition() const
	{
		return position;
	}

	const math::Vec2u &Window::GetSize() const
	{
		return size;
	}

	/*--------------+
	| driver access |
	+--------------*/

	aud::Driver &Window::GetAudioDriver()
	{
		if (!audioDriver)
			audioDriver = MakeAudioDriver();
		return *audioDriver;
	}

	inp::Driver &Window::GetInputDriver()
	{
		if (!inputDriver)
			inputDriver = MakeInputDriver();
		return *inputDriver;
	}

	vid::Driver &Window::GetVideoDriver()
	{
		if (!videoDriver)
			videoDriver = MakeVideoDriver();
		return *videoDriver;
	}

	bool Window::HasAudioDriver() const
	{
		return audioDriver != nullptr;
	}

	bool Window::HasInputDriver() const
	{
		return inputDriver != nullptr;
	}

	bool Window::HasVideoDriver() const
	{
		return videoDriver != nullptr;
	}

	std::unique_ptr<aud::Driver> Window::MakeAudioDriver()
	{
		return GLOBAL(aud::DriverRegistry).Make(*this);
	}

	std::unique_ptr<inp::Driver> Window::MakeInputDriver()
	{
		return GLOBAL(inp::DriverRegistry).Make(*this);
	}

	std::unique_ptr<vid::Driver> Window::MakeVideoDriver()
	{
		return GLOBAL(vid::DriverRegistry).Make(*this);
	}

	/*----------------+
	| signal handlers |
	+----------------*/

	void Window::OnExit()
	{
		std::cout << "exit requested by window" << std::endl;
	}

	void Window::OnFocus(bool focus)
	{
		std::cout << "window focus " << (focus ? "gained" : "lost") << std::endl;
		this->focus = focus;
	}

	void Window::OnMove(const math::Vec2i &position)
	{
		std::cout << "window moved to " << position << std::endl;
		this->position = position;
	}

	void Window::OnSize(const math::Vec2u &size)
	{
		std::cout << "window resized to " << size << std::endl;
		this->size = size;
	}
}}
