#include <algorithm> // max
#include <iomanip> // setw
#include <ostream> // basic_ostream

#include <boost/io/ios_state.hpp> // basic_ios_all_saver

#include "../../log/Indenter.hpp"
#include "../source/FileSource.hpp"
#include "../source/Source.hpp" // Source::Open{Reader,Writer}
#include "../var/BasicVar.hpp" // BasicVar::Get{Key,String}
#include "State.hpp"

namespace page { namespace cfg
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
}}
