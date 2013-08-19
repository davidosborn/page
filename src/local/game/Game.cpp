#include <algorithm> // max, min
#include <functional> // bind
#include <iostream> // cout

#include "../aud/Driver.hpp"
#include "../cache/Cache.hpp" // Cache::{Purge,Update}
#include "../cfg/vars.hpp"
#include "../err/report.hpp" // ReportWarning, std::exception
#include "../inp/Driver.hpp"
#include "../log/Indenter.hpp"
#include "../math/interp.hpp" // HermiteScale
#include "../res/clip/Stream.hpp"
#include "../res/Index.hpp" // GetIndex, Index::Refresh
#include "../res/save/SaverRegistry.hpp"
#include "../script/Driver.hpp"
#include "../sys/process.hpp" // Sleep
#include "../sys/timer/Timer.hpp" // MakeTimer, Timer::{GetDelta,Update}
#include "../util/path/expand.hpp" // ExpandPath
#include "../util/cpp.hpp" // STRINGIZE
#include "../vid/Driver.hpp"
#include "../wnd/Window.hpp"
#include "../wnd/WindowRegistry.hpp"
#include "Game.hpp"
#include "Scene.hpp"
#include "UserInterface.hpp"

// TEST: scripting
#include "../cache/proxy/Resource.hpp"
#include "../res/type/Script.hpp"

namespace page { namespace game
{
	const float
		pauseFadeInDuration  = 1,
		pauseFadeOutDuration = 1,
		pauseFadeExponent    = 1.5;

	// construct/destroy
	Game::Game() : exit(false), timeScale(1), paused(false), pauseLevel(0)
	{
		// NOTE: resources must be indexed before initializing the video
		// driver, which uses GLSL shader resources
		std::cout << "initializing resources" << std::endl;
		{
			log::Indenter indenter;
			res::GetIndex(); // build the resource index
		}
		std::cout << "creating window" << std::endl;
		{
			log::Indenter indenter;
			window = GLOBAL(wnd::WindowRegistry).Make(STRINGIZE(NAME));
			// bind signal handlers
			using namespace std::placeholders;
			window->exitSig.connect(std::bind(&Game::OnExit, this));
			window->focusSig.connect(std::bind(&Game::OnFocus, this, _1));
		}
		std::cout << "initializing video driver" << std::endl;
		{
			log::Indenter indenter;
			vid::Driver &driver(window->GetVideoDriver());
		}
		std::cout << "initializing audio driver" << std::endl;
		{
			log::Indenter indenter;
			aud::Driver &driver(window->GetAudioDriver());
		}
		std::cout << "initializing input driver" << std::endl;
		{
			log::Indenter indenter;
			inp::Driver &driver(window->GetInputDriver());
			// bind signal handlers
			using namespace std::placeholders;
			driver.keySig.connect(std::bind(&Game::OnKey, this, _1));
		}
		std::cout << "initializing script driver" << std::endl;
		{
			log::Indenter indenter;
			scriptDriver.reset(new script::Driver(*this));
		}
		std::cout << "initializing scene" << std::endl;
		{
			log::Indenter indenter;
			scene.reset(new Scene(window->GetAudioDriver()));
			window->GetAudioDriver().Imbue(scene.get());
			window->GetVideoDriver().Imbue(scene.get());
		}
		std::cout << "initializing user interface" << std::endl;
		{
			log::Indenter indenter;
			userInterface.reset(new UserInterface(window->GetAudioDriver()));
			window->GetInputDriver().Imbue(userInterface.get());
			window->GetVideoDriver().Imbue(userInterface.get());
		}
		std::cout << "initializing timer" << std::endl;
		{
			log::Indenter indenter;
			timer.reset(sys::MakeTimer());
		}
		// TEST: scripting
		scriptDriver->Run(*cache::Resource<res::Script>("script/test.lua"));
	}
	Game::~Game()
	{
		GLOBAL(cache::Cache).Purge();
		// FIXME: clipStream may print statistics here if still recording
	}

	// main loop
	void Game::Run()
	{
		timer->Reset();
		std::cout << "entering main loop" << std::endl;
		{
			log::Indenter indenter;
			while (!exit)
			{
				window->Update();
				timer->Update();
				if (window->HasFocus())
				{
					if (float deltaTime = FixDeltaTime(timer->GetDelta()))
					{
						window->GetInputDriver().Update();
						UpdateCursor();
						userInterface->Update(deltaTime);
						UpdatePause(deltaTime);
						if (timeScale)
						{
							/*player.Update();
							UpdatePlayer();
							UpdateScene();
							UpdateMenu();*/
							scriptDriver->Update(deltaTime * timeScale);
							scene->Update(window->GetInputDriver(), deltaTime * timeScale);
						}
						window->GetVideoDriver().Update();
						window->GetAudioDriver().Update(deltaTime);
						UpdateRecording();
						GLOBAL(cache::Cache).Update(deltaTime);
					}
				}
				else sys::Sleep();
			}
		}
	}

	// modifiers
	void Game::Quit()
	{
		exit = true;
	}

	// scene access
	Scene &Game::GetScene()
	{
		assert(scene);
		return *scene;
	}
	const Scene &Game::GetScene() const
	{
		assert(scene);
		return *scene;
	}

	// update
	void Game::UpdateCursor()
	{
		if (window->GetInputDriver().GetCursorMode() == inp::Driver::pointCursorMode)
		{
			math::Vec2 cursorPos(window->GetInputDriver().GetCursorPosition());
			if (!userInterface->UpdateCursor(cursorPos, Aspect(window->GetSize())))
			{
				// FIXME: fall back to game-level cursor interaction
			}
		}
	}
	void Game::UpdatePause(float deltaTime)
	{
		if (paused && pauseLevel < 1 || !paused && pauseLevel)
		{
			pauseLevel = paused ?
				std::min(pauseLevel + deltaTime / pauseFadeInDuration,  1.f) :
				std::max(pauseLevel - deltaTime / pauseFadeOutDuration, 0.f);
			float invSoftPauseLevel = 1 - math::HermiteScale(pauseLevel, pauseFadeExponent);
			window->GetVideoDriver().SetSceneSaturation(invSoftPauseLevel);
			timeScale = invSoftPauseLevel;
		}
	}
	void Game::UpdateRecording()
	{
		if (clipStream)
		{
			// FIXME: grab framebuffer and scale if clipQuality <= .75
			// FIXME: don't box filter during scale if clipQuality <= 50
			clipStream->Write(window->GetVideoDriver().RenderImage(clipStream->GetSize()));
		}
	}

	// timing
	float Game::FixDeltaTime(float deltaTime)
	{
		// lock frame rate when recording
		if (clipStream)
		{
			deltaTime += clipDeltaTime;
			if (deltaTime < 1.f / *CVAR(clipFramerate))
			{
				clipDeltaTime = deltaTime;
				deltaTime = 0;
			}
			else
			{
				deltaTime = 1.f / *CVAR(clipFramerate);
				clipDeltaTime = 0;
			}
		}
		return deltaTime;
	}

	// signal handlers
	void Game::OnExit()
	{
		Quit();
	}
	void Game::OnFocus(bool focus)
	{
		if (focus)
		{
			res::GetIndex().Refresh();
			window->GetAudioDriver().Resume();
		}
		else window->GetAudioDriver().Pause();
	}
	void Game::OnKey(inp::Key key)
	{
		switch (key)
		{
			case inp::backspaceKey: // turn around
			// FIXME: implement
			break;
			case inp::escapeKey: // exit game
			exit = true;
			break;
			case inp::pauseKey: // pause game
			if ((paused = !paused))
			{
				// pause
				// FIXME: waiting for implementation
				//window->GetAudioDriver().Pause(pauseFadeInDuration);
				userInterface->Pause();
			}
			else
			{
				// resume
				// FIXME: waiting for implementation
				//window->GetAudioDriver().Resume(pauseFadeOutDuration);
				userInterface->Resume();
			}
			break;
			case inp::printKey: // take screenshot
			{
				std::cout << "taking screenshot" << std::endl;
				log::Indenter indenter;
				try
				{
					timer->Pause();
					GLOBAL(res::SaverRegistry).Save(
						window->GetVideoDriver().RenderImage(*CVAR(screenshotSize)),
						util::ExpandPath(*CVAR(screenshotFilePath), util::ExpandFlags::withImplicitWildcardSuffix),
						*CVAR(screenshotFormat));
					timer->Resume();
				}
				catch (const std::exception &e)
				{
					err::ReportWarning(e);
				}
			}
			break;
			case inp::recordKey: // record video
			if (!clipStream)
			{
				std::cout << "recording clip" << std::endl;
				log::Indenter indenter;
				clipStream.reset(new res::clip::Stream(
					util::ExpandPath(*CVAR(clipFilePath), util::ExpandFlags::withImplicitWildcardSuffix),
					*CVAR(clipFormat),
					*CVAR(clipVideoResolution),
					*CVAR(clipFramerate),
					clipQuality = *CVAR(clipQuality)));
				clipDeltaTime = 0;
			}
			else
			{
				std::cout << "ending recording" << std::endl;
				log::Indenter indenter;
				clipStream.reset();
			}
			break;
			case inp::tabKey: // toggle camera
			{
				inp::Driver &inputDriver(window->GetInputDriver());
				if (scene->GetCameraMode() == phys::Scene::sceneCameraMode)
				{
					// switch to detail camera
					inputDriver.SetCursorMode(inp::Driver::lookCursorMode);
					scene->UseDetailCamera(inputDriver);
				}
				else
				{
					// switch to scene camera
					inputDriver.SetCursorMode(inp::Driver::pointCursorMode);
					scene->UseSceneCamera();
				}
			}
			break;
		}
	}
}}
