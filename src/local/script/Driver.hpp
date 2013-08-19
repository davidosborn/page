#ifndef    page_local_script_Driver_hpp
#   define page_local_script_Driver_hpp

#	include <memory> // shared_ptr
#	include <unordered_map>
#	include <vector>

#	include "../res/type/script/ScriptFormat.hpp"
#	include "../util/class/special_member_functions.hpp" // Uncopyable
#	include "Router.hpp"

namespace page
{
	namespace game { class Game; }
	namespace res { class Script; }

	namespace script
	{
		class Machine;
		class Process;

		struct Driver : util::Uncopyable<Driver>
		{
			// construct
			explicit Driver(game::Game &);

			// update
			void Update(float deltaTime);

			// execution
			void Run(const res::Script &);

			private:
			typedef std::unordered_map<res::ScriptFormat, std::shared_ptr<Machine>, std::hash<int>> Machines;
			Machines machines;
			typedef std::vector<std::shared_ptr<Process>> Processes;
			Processes processes;
			Router router;
		};
	}
}

#endif
