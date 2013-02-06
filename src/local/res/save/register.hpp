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

#ifndef    page_local_res_save_register_hpp
#   define page_local_res_save_register_hpp

#	include <functional> // bind
#	include <string>
#	include "../../util/functional.hpp" // reinterpret_cast_function
#	include "../../util/typeinfo.hpp" // GetTypeId, std::type_info
#	include "function.hpp" // Referenceable, SaveFunction
#	include "registry.hpp" // RegisterSaver

namespace page
{
	namespace res
	{
#	define REGISTER_SAVER(type, save, ext, fmt, rank) \
		namespace \
		{ \
			SaverRegisterer _reg##save( \
				util::GetTypeId<type>(), \
				std::bind( \
					std::function<void (const type &, std::ostream &)>(save), \
					std::bind( \
						util::reinterpret_cast_function<const type &, const Referenceable &>(), \
						std::placeholders::_1), \
					std::placeholders::_2), \
				ext, fmt, rank); \
		}

		struct SaverRegisterer
		{
			SaverRegisterer(const std::type_info &type, const SaveFunction &save, const std::string &ext, const std::string &fmt, unsigned rank)
				{ RegisterSaver(type, save, ext, fmt, rank); }
		};
	}
}

#endif
