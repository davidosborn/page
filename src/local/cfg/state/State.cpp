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

// C++
#include <algorithm> // max
#include <iomanip> // setw
#include <ostream> // basic_ostream

// Boost
#include <boost/filesystem.hpp> // absolute
#include <boost/io/ios_state.hpp> // basic_ios_all_saver

// local
#include "../../log/Indenter.hpp"
#include "../source/FileSource.hpp"
#include "../source/Source.hpp" // Source::Open{Reader,Writer}
#include "../var/BasicVar.hpp" // BasicVar::Get{Key,String}
#include "State.hpp"

namespace page
{
	namespace cfg
	{
		/*--------------------------+
		| constructors & destructor |
		+--------------------------*/

		State::State()
		{
			// initialize default source list
			sources.push_back(std::make_shared<FileSource>("test"));

			Update();
		}

		State::~State()
		{
			Commit();
		}

		/*-----------------+
		| revision control |
		+-----------------*/

		void State::Update()
		{
			std::list<BasicVar *> varsForUpdate;
			for (auto &var : GetVars())
				if (!var.IsModified())
					varsForUpdate.push_back(&var);

			for (const auto &source : sources)
			{
				auto reader(source->OpenReader());
				for (auto iter(varsForUpdate.begin()); iter != varsForUpdate.end(); ++iter)
				{
					auto &var(**iter);
					auto value(reader->Read(var.GetKey()));
					if (value)
					{
						std::istringstream ss(*value);
						ss >> var;
						var.ClearModified();
						iter = varsForUpdate.erase(iter);
						continue;
					}
				}
			}
		}

		void State::Commit()
		{
			// FIXME: implement
		}

		/*-----------------------+
		| user-friendly printing |
		+-----------------------*/

		template <typename Char, typename CharTraits>
			std::basic_ostream<Char, CharTraits> &operator <<(std::basic_ostream<Char, CharTraits> &os, const State &state)
		{
			// calculate cvar alignment width
			std::streamsize width = 0;
			for (const auto &var : state.GetVars())
				width = std::max<std::streamsize>(var.GetKey().size(), width);

			// set flags for trailing alignment
			boost::io::basic_ios_all_saver<char> iosSaver(os);
			os.setf(std::ios_base::left, std::ios_base::adjustfield);
			os.fill(' ');

			// print cvars
			for (const auto &var : state.GetVars())
				os << std::setw(width) << var.GetKey() << " = " << var.GetString() << '\n';

			return os;
		}

		void Print(const State &state)
		{
			if (*state.logVerbose)
			{
				std::cout << "configuration:\n";
				log::Indenter indenter;
				std::cout << state << std::flush;
			}
		}
	}
}
