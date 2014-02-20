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

#ifndef    page_local_sys_info_hpp
#   define page_local_sys_info_hpp

#	include <string>

#	include <boost/optional.hpp>

namespace page
{
	namespace sys
	{
		/**
		 * Write a summary of the available system information to the standard
		 * output stream.
		 */
		void PrintInfo();

		/**
		 * @return The name of the CPU.
		 */
		std::string GetCpu();

		/**
		 * @return The name of the operating system.
		 */
		std::string GetOs();

		/**
		 * @return The computer's hostname.
		 */
		std::string GetHostname();

		/**
		 * @return The name of the current user.
		 */
		std::string GetUsername();

		/**
		 * @return The path of the home directory.
		 */
		std::string GetHome();

		/**
		 * @param[in] key The key of the desired environment variable.
		 *
		 * @return The value of an environment variable.
		 */
		boost::optional<std::string> GetEnvVar(const std::string &key);
	}
}

#endif
