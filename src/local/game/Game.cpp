/**
 * @section license
 *
 * Copyright (c) 2006-2012 David Osborn
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

#include <algorithm> // max, min
#include <functional> // bind
#include <iostream> // cout

#include "../aud/Driver.hpp"
#include "../cache/Cache.hpp" // Cache::{Purge,Update}
#include "../cfg/vars.hpp"
#include "../clip/Stream.hpp"
#include "../err/report.hpp" // ReportWarning, std::exception
#include "../inp/Driver.hpp"
#include "../log/Indenter.hpp"
#include "../math/interp.hpp" // HermiteScale
#include "../res/Index.hpp" // GetIndex, Index::Refresh
#include "../res/save.hpp" // Save
#include "../script/Driver.hpp"
#include "../sys/process.hpp" // Sleep
#include "../sys/timer/Timer.hpp" // MakeTimer, Timer::{GetDelta,Update}
#include "../util/path/expand.hpp" // ExpandPath
#include "../util/pp.hpp" // STRINGIZE
#include "../vid/Driver.hpp"
#include "../wnd/Window.hpp"
#include "Game.hpp"
#include "Interface.hpp"
#include "Scene.hpp"

// TEST: scripting
#include "../cache/proxy/Resource.hpp"
#include "../res/type/Script.hpp"

namespace page
{
	namespace game
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
				wnd.reset(wnd::MakeWindow(STRINGIZE(NAME)));
				// bind signal handlers
				using namespace std::placeholders;
				wnd->exitSig.Connect(std::bind(&Game::OnExit, this));
				wnd->focusSig.Connect(std::bind(&Game::OnFocus, this, _1));
			}
			std::cout << "initializing video driver" << std::endl;
			{
				log::Indenter indenter;
				vid::Driver &driver(wnd->GetVideoDriver());
			}
			std::cout << "initializing audio driver" << std::endl;
			{
				log::Indenter indenter;
				aud::Driver &driver(wnd->GetAudioDriver());
			}
			std::cout << "initializing input driver" << std::endl;
			{
				log::Indenter indenter;
				inp::Driver &driver(wnd->GetInputDriver());
				// bind signal handlers
				using namespace std::placeholders;
				driver.keySig.Connect(std::bind(&Game::OnKey, this, _1));
			}
			std::cout << "initializing script driver" << std::endl;
			{
				log::Indenter indenter;
				scriptDriver.reset(new script::Driver(*this));
			}
			std::cout << "initializing scene" << std::endl;
			{
				log::Indenter indenter;
				scene.reset(new Scene(wnd->GetAudioDriver()));
				wnd->GetAudioDriver().Imbue(scene.get());
				wnd->GetVideoDriver().Imbue(scene.get());
			}
			std::cout << "initializing user interface" << std::endl;
			{
				log::Indenter indenter;
				interface.reset(new Interface(wnd->GetAudioDriver()));
				wnd->GetInputDriver().Imbue(interface.get());
				wnd->GetVideoDriver().Imbue(interface.get());
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
					wnd->Update();
					timer->Update();
					if (wnd->HasFocus())
					{
						if (float deltaTime = FixDeltaTime(timer->GetDelta()))
						{
							wnd->GetInputDriver().Update();
							UpdateCursor();
							interface->Update(deltaTime);
							UpdatePause(deltaTime);
							if (timeScale)
							{
//								player.Update();
//								UpdatePlayer();
//								UpdateScene();
//								UpdateMenu();
								scriptDriver->Update(deltaTime * timeScale);
								scene->Update(wnd->GetInputDriver(), deltaTime * timeScale);
							}
							wnd->GetVideoDriver().Update();
							wnd->GetAudioDriver().Update(deltaTime);
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
			if (wnd->GetInputDriver().GetCursorMode() == inp::Driver::pointCursorMode)
			{
				math::Vector<2> cursorPos(wnd->GetInputDriver().GetCursorPosition());
				if (!interface->UpdateCursor(cursorPos, Aspect(wnd->GetSize())))
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
				wnd->GetVideoDriver().SetSceneSaturation(invSoftPauseLevel);
				timeScale = invSoftPauseLevel;
			}
		}
		void Game::UpdateRecording()
		{
			if (clipStream)
			{
				// FIXME: grab framebuffer and scale if clipQuality <= .75
				// FIXME: don't box filter during scale if clipQuality <= 50
				clipStream->Write(wnd->GetVideoDriver().RenderImage(clipStream->GetSize()));
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
				wnd->GetAudioDriver().Resume();
			}
			else wnd->GetAudioDriver().Pause();
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
//					wnd->GetAudioDriver().Pause(pauseFadeInDuration);
					interface->Pause();
				}
				else
				{
					// resume
					// FIXME: waiting for implementation
//					wnd->GetAudioDriver().Resume(pauseFadeOutDuration);
					interface->Resume();
				}
				break;
				case inp::printKey: // take screenshot
				{
					std::cout << "taking screenshot" << std::endl;
					log::Indenter indenter;
					try
					{
						timer->Pause();
						Save(
							wnd->GetVideoDriver().RenderImage(*CVAR(screenshotSize)),
							util::ExpandPath(*CVAR(screenshotFilePath), util::ExpandFlags::withImplicitWildcardSuffix).native(),
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
					std::string path(util::ExpandPath(*CVAR(clipFilePath) + '*'));
					path.erase(path.size() - 1);
					clipStream.reset(new clip::Stream(path,
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
					inp::Driver &inputDriver(wnd->GetInputDriver());
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
	}
}
