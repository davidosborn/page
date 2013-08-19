#ifndef    page_local_game_Game_hpp
#   define page_local_game_Game_hpp

#	include <memory> // unique_ptr

#	include "../inp/Key.hpp"
#	include "../util/class/special_member_functions.hpp" // Uncopyable

namespace page
{
	namespace clip { class Stream; }
	namespace script { class Driver; }
	namespace sys { class Timer; }
	namespace wnd { class Window; }
}

namespace page { namespace game
{
	class Scene;
	class UserInterface;

	struct Game : util::Uncopyable<Game>
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

		std::unique_ptr<wnd::Window> window;
		std::unique_ptr<script::Driver> scriptDriver;
		std::unique_ptr<Scene> scene;
		std::unique_ptr<UserInterface> userInterface;
		std::unique_ptr<sys::Timer> timer;
		bool exit;
		float timeScale;
		bool paused;
		float pauseLevel;
		std::unique_ptr<res::clip::Stream> clipStream;
		float clipDeltaTime;
		float clipQuality;
	};
}}

#endif
