#include <algorithm> // find
#include <array>
#include <cstdlib> // exit
#include <iostream> // cout
#include <tuple> // tie

#include "../util/cpp.hpp" // STRINGIZE
#include "../util/path/filesystem.hpp" // Basename
#include "../util/string/operations.hpp" // Partition
#include "CmdlineParser.hpp"

namespace page { namespace cfg
{
	namespace
	{
		/**
		 * A string of the short options having arguments.
		 */
		const std::string optsWithArgs("c");

		/**
		 * An array of the long options having arguments.
		 */
		const std::array<std::string, 1> longOptsWithArgs =
		{
			"config"
		};
	}

	/*--------+
	| parsing |
	+--------*/

	void CmdlineParser::Parse(int argc, char *argv[])
	{
		const auto &arg0(argv[0]);

		// parse out options with their arguments
		std::vector<std::pair<std::string, std::string>> optArgs;
		const char *const *argp = argv + 1;
		for (; argp != argv + argc; ++argp)
		{
			if ((*argp)[0] != '-' || !(*argp)[1]) break;
			if ((*argp)[1] != '-')
			{
				// short option(s)
				for (const char *optp = *argp + 1; *optp; ++optp)
				{
					std::string opt(std::string(1, '-') + *optp), arg;
					if (optsWithArgs.find(*optp) != optsWithArgs.npos)
						if (optp[1])
						{
							arg = optp + 1;
							break;
						}
						else if ((arg = *++argp).empty())
							PrintErrorMissingArgument(arg0, opt);
					optArgs.push_back(std::make_pair(opt, arg));
				}
			}
			else if ((*argp)[2])
			{
				// long option
				std::string opt, arg;
				std::tie(opt, arg) = util::Partition(*argp, util::SequenceDeserializationFlags::none, '=');
				if (std::find(longOptsWithArgs.begin(), longOptsWithArgs.end(), opt.substr(2)) != longOptsWithArgs.end())
				{
					if (arg.empty() && (arg = *++argp).empty())
						PrintErrorMissingArgument(arg0, opt);
				}
				else if (!arg.empty())
					PrintErrorUnexpectedArgument(arg0, opt, arg);
				optArgs.push_back(std::make_pair(opt, arg));
			}
			else
			{
				// explicit end of options
				argp++;
				break;
			}
		}

		// evaluate parsed out options
		for (const auto &optArg : optArgs)
		{
			const std::string
				&opt(optArg.first),
				&arg(optArg.second);
			if      (opt == "-c" || opt == "--config")  cfgSources.push_back(arg);
			if      (opt == "-d" || opt == "--data")    resSources.push_back(arg);
			else if (opt == "-h" || opt == "--help")    PrintUsage(arg0);
			else if (opt == "-q" || opt == "--quiet")   cfgVars["log.quiet"] = "true";
			else if (opt == "-s" || opt == "--sync")    cfgVars["log.sync"] = "true";
			else if (opt == "-v" || opt == "--verbose") cfgVars["log.verbose"] = "true";
			else if (opt == "-V" || opt == "--version") PrintVersion();
			else PrintErrorUnrecognizedOption(arg0, opt);
		}

		// parse configuration variables
		for (; argp != argv + argc; ++argp)
		{
			std::string opt, arg;
			std::tie(opt, arg) = util::Partition(*argp, util::SequenceDeserializationFlags::none, '=');
			if (arg.empty()) break;
			cfgVars[opt] = arg;
		}

		// parse resource sources
		for (; argp != argv + argc; ++argp)
			resSources.push_back(*argp);
	}

	/*---------------+
	| parsing output |
	+---------------*/

	void PrintUsage(const std::string &arg0)
	{
		std::cout << "Usage: " << arg0 << " [options] [cvars] [archives]" << std::endl;
		std::cout << "Options:" << std::endl;
		std::cout << "  -c --config=FILE   Read configuration from FILE" << std::endl;
		std::cout << "  -d --data=FILE     Load content from FILE" << std::endl;
		std::cout << "  -h --help          Print command line usage information" << std::endl;
		std::cout << "  -q --quiet         Suppress progress information" << std::endl;
		std::cout << "  -s --sync          Synchronize stdout and stderr" << std::endl;
		std::cout << "  -v --verbose       Print as much information as possible" << std::endl;
		std::cout << "  -V --version       Print program version information" << std::endl;
		std::cout << std::endl;
		std::cout << "Report bugs to <" << STRINGIZE(BUGS) << ">" << std::endl;
		std::exit(0);
	}

	void PrintVersion()
	{
		std::cout << STRINGIZE(NAME) " " STRINGIZE(VERSION) ": " STRINGIZE(LONGNAME) << std::endl;
		std::cout << STRINGIZE(COPYRIGHT) << std::endl;
		std::exit(0);
	}

	void PrintMoreInfo(const std::string &arg0)
	{
		std::cout << "Try `" << arg0 << " --help' for more information." << std::endl;
	}

	void PrintErrorUnrecognizedOption(const std::string &arg0, const std::string &opt)
	{
		std::cout << util::Basename(arg0) << ": unrecognized option `" << opt << "'" << std::endl;
		PrintMoreInfo(arg0);
		std::exit(1);
	}

	void PrintErrorInvalidArgument(const std::string &arg0, const std::string &opt, const std::string &arg)
	{
		std::cout << util::Basename(arg0) << ": invalid argument `" << arg << "' to `" << opt << "'" << std::endl;
		std::cout << "" << std::endl;
		PrintMoreInfo(arg0);
		std::exit(1);
	}

	void PrintErrorUnexpectedArgument(const std::string &arg0, const std::string &opt, const std::string &arg)
	{
		std::cout << util::Basename(arg0) << ": unexpected argument `" << arg << "' to `" << opt << "'" << std::endl;
		PrintMoreInfo(arg0);
		std::exit(1);
	}

	void PrintErrorMissingArgument(const std::string &arg0, const std::string &opt)
	{
		std::cout << util::Basename(arg0) << ": missing argument to `" << opt << "'" << std::endl;
		PrintMoreInfo(arg0);
		std::exit(1);
	}
}}
