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

#include <algorithm> // find
#include <array>
#include <cstdlib> // exit
#include <iostream> // cout
#include <tuple> // tie
#include "opt.hpp" // Cfg{Sources,Vars}, ResSources
#include "sys/file.hpp" // BaseName
#include "util/pp.hpp" // STRINGIZE
#include "util/string.hpp" // Partition

namespace page
{
	namespace opt
	{
		CfgSources cfgSources;
		CfgVars cfgVars;
		ResSources resSources;

		namespace
		{
			const std::string optsWithArgs("c");
			const std::array<std::string, 1> longOptsWithArgs =
			{
				"config"
			};

			void Usage(const std::string &exec)
			{
				std::cout << "Usage: " << exec << " [options] [cvars] [archives]" << std::endl;
				std::cout << "Options:" << std::endl;
				std::cout << "  -c --config=FILE   Read configuration from FILE" << std::endl;
				std::cout << "  -h --help          Print command line usage information" << std::endl;
				std::cout << "  -q --quiet         Suppress progress information" << std::endl;
				std::cout << "  -s --sync          Synchronize stdout and stderr" << std::endl;
				std::cout << "  -v --verbose       Print as much information as possible" << std::endl;
				std::cout << "  -V --version       Print program version information" << std::endl;
				std::cout << std::endl;
				std::cout << "Report bugs to <" << STRINGIZE(BUGS) << ">" << std::endl;
				std::exit(0);
			}
			void Version()
			{
				std::cout << STRINGIZE(NAME) " " STRINGIZE(VERSION) ": " STRINGIZE(LONGNAME) << std::endl;
				std::cout << STRINGIZE(COPYRIGHT) << std::endl;
				std::exit(0);
			}
			void MoreInfo(const std::string &exec)
			{
				std::cout << "Try `" << exec << " --help' for more information." << std::endl;
			}
			void BadOption(const std::string &exec, const std::string &opt)
			{
				std::cout << sys::BaseName(exec) << ": unrecognized option `" << opt << "'" << std::endl;
				MoreInfo(exec);
				std::exit(1);
			}
			void BadArgument(const std::string &exec, const std::string &opt, const std::string &arg)
			{
				std::cout << sys::BaseName(exec) << ": invalid argument `" << arg << "' to `" << opt << "'" << std::endl;
				std::cout << "" << std::endl;
				MoreInfo(exec);
				std::exit(1);
			}
			void OddArgument(const std::string &exec, const std::string &opt, const std::string &arg)
			{
				std::cout << sys::BaseName(exec) << ": unexpected argument `" << arg << "' to `" << opt << "'" << std::endl;
				MoreInfo(exec);
				std::exit(1);
			}
			void NoArgument(const std::string &exec, const std::string &opt)
			{
				std::cout << sys::BaseName(exec) << ": missing argument to `" << opt << "'" << std::endl;
				MoreInfo(exec);
				std::exit(1);
			}
		}

		void Parse(int argc, char *argv[])
		{
			const char *exec = argv[0];
			// parse out options with their arguments
			typedef std::vector<std::pair<std::string, std::string>> OptArgs;
			OptArgs optArgs;
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
								NoArgument(exec, opt);
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
							NoArgument(exec, opt);
					}
					else if (!arg.empty()) OddArgument(exec, opt, arg);
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
			for (OptArgs::const_iterator optArg(optArgs.begin()); optArg != optArgs.end(); ++optArg)
			{
				const std::string &opt(optArg->first), &arg(optArg->second);
				if (opt == "-c" || opt == "--config") cfgSources.push_back(arg);
				else if (opt == "-h" || opt == "--help") Usage(exec);
				else if (opt == "-q" || opt == "--quiet") cfgVars["log.quiet"] = "true";
				else if (opt == "-s" || opt == "--sync") cfgVars["log.sync"] = "true";
				else if (opt == "-v" || opt == "--verbose") cfgVars["log.verbose"] = "true";
				else if (opt == "-V" || opt == "--version") Version();
				else BadOption(exec, opt);
			}
			// parse configuration variables
			for (; argp != argv + argc; ++argp)
			{
				std::string opt, arg;
				std::tie(opt, arg) = util::Partition(*argp, util::SequenceDeserializationFlags::none, '=');
				if (arg.empty()) break;
				cfgVars[opt] = arg;
			}
			// parse archives
			for (; argp != argv + argc; ++argp)
				resSources.push_back(*argp);
		}
	}
}
