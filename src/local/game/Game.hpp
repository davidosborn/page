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

#ifndef    page_local_game_Game_hpp
#   define page_local_game_Game_hpp

#	include <memory> // unique_ptr
#	include "../inp/Key.hpp"
#	include "../util/NonCopyable.hpp"

namespace page
{
	namespace clip { class Stream; }
	namespace env { class Window; }
	namespace script { class Driver; }
	namespace sys { class Timer; }

	namespace game
	{
		class Interface;
		class Scene;

		struct Game : util::NonCopyable
		{
			// construct/destroy
			Game();
			~Game();

			// main loop
			void Run();

			// modifiers
			void Quit();

			// scene access
			Scene &GetScene();
			const Scene &GetScene() const;

			private:
			// update
			void UpdateCursor();
			void UpdatePause(float deltaTime);
			void UpdateRecording();

			// timing
			float FixDeltaTime(float);

			// signal handlers
			void OnExit();
			void OnFocus(bool);
			void OnKey(inp::Key);

			std::unique_ptr<env::Window> wnd;
			std::unique_ptr<script::Driver> scriptDriver;
			std::unique_ptr<Scene> scene;
			std::unique_ptr<Interface> interface;
			std::unique_ptr<sys::Timer> timer;
			bool exit;
			float timeScale;
			bool paused;
			float pauseLevel;
			std::unique_ptr<clip::Stream> clipStream;
			float clipDeltaTime;
			float clipQuality;
		};
	}
}

#endif
