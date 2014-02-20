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

#ifndef    page_local_cfg_Options_hpp
#   define page_local_cfg_Options_hpp

#	include <string>
#	include <unordered_map>
#	include <vector>

#	include "../util/class/Monostate.hpp"

namespace page { namespace cfg
{
	class CmdlineParser : public util::Monostate<CmdlineParser>
	{
		/*----------+
		| observers |
		+----------*/

		public:
		/**
		 * @todo Use @c decltype(auto) in C++14.
		 */
		const std::vector<std::string> &GetCfgSources() const
		{
			return cfgSources;
		}

		/**
		 * @todo Use @c decltype(auto) in C++14.
		 */
		const std::unordered_map<std::string, std::string> &GetCfgVars() const
		{
			return cfgVars;
		}

		/**
		 * @todo Use @c decltype(auto) in C++14.
		 */
		const std::vector<std::string> &GetResSources() const
		{
			return resSources;
		}

		/*--------+
		| parsing |
		+--------*/

		/**
		 * Parses the given command-line arguments.
		 */
		void Parse(int argc, char *argv[]);

		/*---------------+
		| parsing output |
		+---------------*/

		private:
		/**
		 * Prints the usage information.
		 */
		void PrintUsage(const std::string &arg0);

		/**
		 * Prints the version.
		 */
		void PrintVersion();

		/**
		 * Prints instructions on getting more information.
		 */
		void PrintMoreInfo(const std::string &arg0);

		/**
		 * Prints an error message for an unrecognized option.
		 */
		void PrintErrorUnrecognizedOption(
			const std::string &arg0,
			const std::string &opt);

		/**
		 * Prints an error message for an invalid argument.
		 */
		void PrintErrorInvalidArgument(
			const std::string &arg0,
			const std::string &opt,
			const std::string &arg);

		/**
		 * Prints an error message for an unexpected argument.
		 */
		void PrintErrorUnexpectedArgument(
			const std::string &arg0,
			const std::string &opt,
			const std::string &arg);

		/**
		 * Prints an error message for a missing argument.
		 */
		void PrintErrorMissingArgument(
			const std::string &arg0,
			const std::string &opt);

		/*-------------+
		| data members |
		+-------------*/

		/**
		 * A list of configuration sources.
		 */
		std::vector<std::string> cfgSources;

		/**
		 * An associative array of configuration variables.
		 */
		std::unordered_map<std::string, std::string> cfgVars;

		/**
		 * A list of resource sources.
		 */
		std::vector<std::string> resSources;
	};
}}

#endif
