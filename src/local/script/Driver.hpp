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

#ifndef    page_local_script_Driver_hpp
#   define page_local_script_Driver_hpp

#	include <memory> // shared_ptr
#	include <unordered_map>
#	include <vector>
#	include "../res/type/script/ScriptFormat.hpp"
#	include "../util/NonCopyable.hpp"
#	include "Router.hpp"

namespace page
{
	namespace game { class Game; }
	namespace res { class Script; }

	namespace script
	{
		class Machine;
		class Process;

		struct Driver : util::NonCopyable
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
