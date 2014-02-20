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

#ifndef    page_local_game_Game_hpp
#   define page_local_game_Game_hpp

#	include <memory> // unique_ptr

#	include "../aud/SoundProxy.hpp"
#	include "../inp/Key.hpp"
#	include "../util/class/special_member_functions.hpp" // Polymorphic, Uncopyable

namespace page
{
	namespace gui
	{
		class DebugWindow;
		class Root;
		class SpeechWindow;
	}
	namespace phys { class Scene; }
	namespace res
	{
		namespace clip { class Stream; }
		class Scene;
	}
	namespace script { class Driver; }
	namespace sys { class Timer; }
	namespace wnd { class Window; }
}

namespace page { namespace game
{
	class Player;

	/**
	 * The game.
	 */
	class Game :
		public util::Polymorphic<Game>,
		public util::Uncopyable<Game>
	{
		/*-------------+
		| constructors |
		+-------------*/

		public:
		Game();
		~Game();

		/*----------+
		| modifiers |
		+----------*/

		/**
		 * Loads the specified scene.
		 */
		void LoadScene(const res::Scene &);

		void Quit();

		/*----------+
		| main loop |
		+----------*/

		/**
		 * The main loop.
		 */
		void Run();

		private:
		/**
		 * Provides the GUI with an updated cursor position, which is used to
		 * recognize when the cursor is hovering over a widget.
		 */
		void UpdateCursor();

		/**
		 * Updates the state of the smooth-pausing operation, if it is currently
		 * in progress.
		 */
		void UpdatePause(float deltaTime);

		/**
		 * Grabs the next frame of video when recording and writes it to the
		 * clip stream.
		 */
		void UpdateRecording();

		/*-------+
		| timing |
		+-------*/

		/**
		 * Locks the frame rate when recording.
		 */
		float FixDeltaTime(float);

		/*----------------+
		| signal handlers |
		+----------------*/

		void OnExit();
		void OnFocus(bool);
		void OnKey(inp::Key);

		/*-------------+
		| data members |
		+-------------*/

		/**
		 * The main window.
		 */
		std::unique_ptr<wnd::Window> window;

		/**
		 * The script driver.
		 */
		std::unique_ptr<script::Driver> scriptDriver;

		/**
		 * The timer.
		 */
		std::unique_ptr<sys::Timer> timer;

		/**
		 * The player.
		 */
		std::unique_ptr<Player> player;

		/**
		 * The scene.
		 */
		std::unique_ptr<phys::Scene> scene;

		/**
		 * The user interface.
		 */
		//std::unique_ptr<gui::Root> gui;

		/**
		 * @c true if we are in the process of quitting the game.
		 */
		bool exit = false;

		/**
		 * The current time-scale, which is used to smoothly pause and unpause
		 * the game.
		 */
		float timeScale = 1;

		/**
		 * @c true if the game is paused.
		 */
		bool paused = false;

		/**
		 * The current pause scale, which is a unit value representing the
		 * fading state for smoothly pausing and unpausing the game.
		 */
		float pauseScale = 0;

		/**
		 * The clip stream for recording.
		 */
		std::unique_ptr<res::clip::Stream> clipStream;

		/**
		 * The time step used to lock the frame rate when recording.
		 */
		float clipDeltaTime;

		/**
		 * The video quality to use to use when recording.
		 */
		float clipQuality;

		/**
		 * The background music.
		 */
		aud::SoundProxy music;

		/**
		 * The debug window.
		 */
		//std::unique_ptr<gui::DebugWindow> debugWindow;

		/**
		 * The speech window.
		 */
		//std::unique_ptr<gui::SpeechWindow> speechWindow;
	};
}}

#endif
