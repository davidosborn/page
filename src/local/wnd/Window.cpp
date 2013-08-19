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

namespace page { namespace wnd
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
	const math::Vec2i &Window::GetPosition() const
	{
		return pos;
	}
	const math::Vec2u &Window::GetSize() const
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
		const math::Vec2i &pos,
		const math::Vec2u &size)
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
	void Window::OnMove(const math::Vec2i &pos)
	{
		std::cout << "window moved to " << pos << std::endl;
		this->pos = pos;
	}
	void Window::OnSize(const math::Vec2u &size)
	{
		std::cout << "window resized to " << size << std::endl;
		this->size = size;
	}
}}
