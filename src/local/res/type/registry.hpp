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

#ifndef    page_local_res_type_registry_hpp
#   define page_local_res_type_registry_hpp

#	include <string>
#	include <typeinfo> // type_info

#	include "../../util/memory/Deleter.hpp" // Deleter
#	include "function.hpp" // PostLoadFunction

namespace page
{
	namespace res
	{
		// access
		/**
		 * Return the registered name of a given resource type.
		 */
		const std::string &GetRegisteredTypeName(const std::type_info &);
		/**
		 * Return the registered deleter of a given resource type.
		 */
		const util::Deleter &GetRegisteredTypeDeleter(const std::type_info &);
		/**
		 * Return the registered post-loader of a given resource type.
		 */
		const PostLoadFunction &GetRegisteredTypePostLoader(const std::type_info &);

		// registration
		/**
		 * Register a resource type with the system.
		 */
		void RegisterType(const std::type_info &, const std::string &, const util::Deleter &, const PostLoadFunction &);
	}
}

#endif
