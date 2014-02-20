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

#include <cassert>
#include <unordered_map>

#include "../../err/Exception.hpp"
#include "registry.hpp" // MachineFactory

namespace page
{
	namespace script
	{
		namespace
		{
			struct Registry
			{
				typedef std::unordered_map<res::ScriptFormat, MachineFactory, std::hash<int>> Factories;
				Factories factories;
			};
			inline Registry &GetRegistry()
			{
				static Registry reg;
				return reg;
			}
		}

		// creation
		Machine *MakeMachine(res::ScriptFormat format, Router &router)
		{
			const Registry &reg(GetRegistry());
			Registry::Factories::const_iterator iter(reg.factories.find(format));
			if (iter != reg.factories.end()) return iter->second(router);
			THROW((err::Exception<err::ScriptModuleTag, err::NotAvailableTag>("no " + GetName(format) + " machine available")))
		}

		// registration
		void RegisterMachine(const MachineFactory &factory, res::ScriptFormat format)
		{
			assert(factory);
			GetRegistry().factories.insert(std::make_pair(format, factory));
		}
	}
}
