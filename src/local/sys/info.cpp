#include <iostream>

#include "../cfg/vars.hpp"
#include "../log/Indenter.hpp"
#include "info.hpp"

namespace page
{
	namespace sys
	{
		void PrintInfo()
		{
			std::cout << "system information" << std::endl;
			log::Indenter indenter;
			if (*CVAR(logHost))
			{
				try
				{
					std::string
						user(GetUsername()),
						host(GetHostname());
					std::cout << "host is " << user << " on " << host << std::endl;
				}
				catch (...) {}
			}
			try
			{
				std::string platform(GetOs());
				std::cout << "platform is " << platform << std::endl;
			}
			catch (...) {}
			try
			{
				std::string machine(GetCpu());
				std::cout << "machine is " << machine << std::endl;
			}
			catch (...) {}
		}
	}
}
