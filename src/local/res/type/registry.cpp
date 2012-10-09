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

#include <cassert>
#include <string>
#include <typeinfo> // type_info::name
#include <unordered_map>
#include "../../util/Deleter.hpp" // Deleter
#include "function.hpp" // PostLoadFunction

namespace page
{
	namespace res
	{
		namespace
		{
			struct Registry
			{
				struct Type
				{
					std::string name;
					util::Deleter deleter;
					PostLoadFunction postLoad;
				};
				typedef std::unordered_map<std::string, Type> Types;
				Types types;
			};
			inline Registry &GetRegistry()
			{
				static Registry reg;
				return reg;
			}
		}

		// access
		const std::string &GetRegisteredTypeName(const std::type_info &id)
		{
			const Registry &reg(GetRegistry());
			Registry::Types::const_iterator iter(reg.types.find(id.name()));
			assert(iter != reg.types.end());
			const Registry::Type &type(iter->second);
			return type.name;
		}
		const util::Deleter &GetRegisteredTypeDeleter(const std::type_info &id)
		{
			const Registry &reg(GetRegistry());
			Registry::Types::const_iterator iter(reg.types.find(id.name()));
			assert(iter != reg.types.end());
			const Registry::Type &type(iter->second);
			return type.deleter;
		}
		const PostLoadFunction &GetRegisteredTypePostLoader(const std::type_info &id)
		{
			const Registry &reg(GetRegistry());
			Registry::Types::const_iterator iter(reg.types.find(id.name()));
			assert(iter != reg.types.end());
			const Registry::Type &type(iter->second);
			return type.postLoad;
		}

		// registration
		void RegisterType(const std::type_info &id, const std::string &name, const util::Deleter &deleter, const PostLoadFunction &postLoad)
		{
			Registry &reg(GetRegistry());
			Registry::Type type = {name, deleter, postLoad};
			reg.types.insert(std::make_pair(id.name(), type));
		}
	}
}
