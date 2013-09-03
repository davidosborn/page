#ifndef    page_local_game_UserInterface_hpp
#   define page_local_game_UserInterface_hpp

#	include <memory> // shared_ptr

#	include "../gui/UserInterface.hpp"

namespace page
{
	namespace aud { class Driver; }
	namespace ui
	{
		class Array;
		class Text;
		class Window;
	}
}

namespace page { namespace game
{
	struct UserInterface : ui::UserInterface
	{
		// construct
		UserInterface(aud::Driver &);

		// update
		void Update(float deltaTime);

		// modifiers
		void Pause();
		void Resume();

		private:
		aud::Driver &audioDriver;
		std::shared_ptr<ui::Text>
			statRunTime,
			statFrameCount,
			statFrameRate,
			// FIXME: waiting on support for video statistics
			/*statTriCount,
			statTriRate,
			statPixelCount,
			statPixelRate,*/
			statCacheTries,
			statCacheMisses,
			statCacheCoherence;
		std::shared_ptr<ui::Window> statWindow;
		std::shared_ptr<ui::Array> cmdBar;
	};
}}

#endif
