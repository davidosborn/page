/**
 * @section license
 *
 * Copyright (c) 2006-2013 David Osborn
 *
 * Permission is granted to use and redistribute this software in source and
 * binary form, with or without modification, subject to the following
 * conditions:
 *
 * 1. Redistributions in source form must retain the above copyright notice,
 *    this list of conditions, and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions, and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution, and in the same
 *    place and form as other copyright, license, and disclaimer information.
 *
 * As a special exception, distributions of derivative works in binary form may
 * include an acknowledgement in place of the above copyright notice, this list
 * of conditions, and the following disclaimer in the documentation and/or other
 * materials provided with the distribution, and in the same place and form as
 * other acknowledgements, similar in substance to the following:
 *
 *    Portions of this software are based on the work of David Osborn.
 *
 * This software is provided "as is", without any express or implied warranty.
 * In no event will the authors be liable for any damages arising out of the use
 * of this software.
 */

#include <functional> // bind
#include <iostream> // cout

#include "../aud/Driver.hpp" // Driver::~Driver
#include "../aud/NullDriver.hpp" // NullDriver::NullDriver
#include "../inp/Driver.hpp" // Driver::~Driver
#include "../vid/Driver.hpp" // Driver::~Driver
#include "Window.hpp"

#ifdef HAVE_AUDIO
#	include "../err/report.hpp" // ReportWarning, std::exception
#endif

namespace page
{
	namespace wnd
	{
		// construct/destroy
		Window::Window() : focus(false)
		{
			using namespace std::placeholders;
			exitSig.connect(std::bind(&Window::OnExit, this));
			focusSig.connect(std::bind(&Window::OnFocus, this, _1));
			moveSig.connect(std::bind(&Window::OnMove, this, _1));
			sizeSig.connect(std::bind(&Window::OnSize, this, _1));
		}
		Window::~Window() {}

		// state
		bool Window::HasFocus() const
		{
			return focus;
		}
		const math::Vector<2, int> &Window::GetPosition() const
		{
			return pos;
		}
		const math::Vector<2, unsigned> &Window::GetSize() const
		{
			return size;
		}

		// driver access
		aud::Driver &Window::GetAudioDriver()
		{
			if (!audioDriver)
			{
#ifdef HAVE_AUDIO
				try
				{
					audioDriver.reset(MakeAudioDriver());
					goto Ready;
				}
				catch (const std::exception &e)
				{
					err::ReportWarning(e);
				}
#endif
				audioDriver.reset(new aud::NullDriver(*this));
			}
			Ready:
			return *audioDriver;
		}
		inp::Driver &Window::GetInputDriver()
		{
			if (!inputDriver) inputDriver.reset(MakeInputDriver());
			return *inputDriver;
		}
		vid::Driver &Window::GetVideoDriver()
		{
			if (!videoDriver) videoDriver.reset(MakeVideoDriver());
			return *videoDriver;
		}

		// deferred state initialization
		void Window::InitState(bool focus,
			const math::Vector<2, int> &pos,
			const math::Vector<2, unsigned> &size)
		{
			this->focus = focus;
			this->pos = pos;
			this->size = size;
		}

		// preemptive destruction
		void Window::Deinit()
		{
			audioDriver.reset();
			inputDriver.reset();
			videoDriver.reset();
		}

		// driver state
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

		// signal handlers
		void Window::OnExit()
		{
			std::cout << "exit requested by window" << std::endl;
		}
		void Window::OnFocus(bool focus)
		{
			std::cout << "window focus " << (focus ? "gained" : "lost") << std::endl;
			this->focus = focus;
		}
		void Window::OnMove(const math::Vector<2, int> &pos)
		{
			std::cout << "window moved to " << pos << std::endl;
			this->pos = pos;
		}
		void Window::OnSize(const math::Vector<2, unsigned> &size)
		{
			std::cout << "window resized to " << size << std::endl;
			this->size = size;
		}
	}
}
