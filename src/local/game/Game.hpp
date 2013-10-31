#ifndef    page_local_game_Game_hpp
#   define page_local_game_Game_hpp

#	include <memory> // unique_ptr

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
