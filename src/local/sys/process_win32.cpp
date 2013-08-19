#include <windows.h> // Sleep

namespace page
{
	namespace sys
	{
		void Sleep()
		{
			::Sleep(0);
		}
	}
}
