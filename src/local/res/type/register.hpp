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

#ifndef    page_local_res_type_register_hpp
#   define page_local_res_type_register_hpp

#	include <functional> // bind, function
#	include <string>

#	include "../../util/class/typeinfo.hpp" // GetIncompleteTypeInfo, std::type_info
#	include "../../util/Deleter.hpp" // Deleter, GetDeleter
#	include "../../util/functional/cast.hpp" // reinterpret_cast_function
#	include "function.hpp" // PostLoadFunction, Referenceable
#	include "registry.hpp" // RegisterType

namespace page
{
	namespace res
	{
#	define REGISTER_TYPE(type, name, postLoad) \
		namespace \
		{ \
			TypeRegisterer _reg##type( \
				util::GetIncompleteTypeInfo<type>(), name, util::GetDeleter<type>(), \
				postLoad ? \
				std::bind( \
					std::function<void (type &)>(postLoad), \
					std::bind( \
						util::reinterpret_cast_function<type &, Referenceable &>(), \
						std::placeholders::_1)) : \
				PostLoadFunction()); \
		}

		struct TypeRegisterer
		{
			TypeRegisterer(const std::type_info &type, const std::string &name, const util::Deleter &deleter, const PostLoadFunction &postLoad)
				{ RegisterType(type, name, deleter, postLoad); }
		};
	}
}

#endif
