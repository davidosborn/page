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
