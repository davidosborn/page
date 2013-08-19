#include <iostream> // c{err,log,out}, ios_base::Init, streambuf
#include <memory> // shared_ptr

#include "../util/gcc/init_priority.hpp" // LOG_INIT_PRIORITY
#include "sink/ConsoleSink.hpp"
#include "stream/BranchableStream.hpp"

namespace page
{
	namespace log
	{
		namespace
		{
			/**
			 * An alternative initializer for the logging system, which is
			 * simpler than the default one -- it has fewer dependencies and it
			 * doesn't recognize any configuration variables.  It is designed
			 * for unit testing when console output is required.
			 */
			class Initializer
			{
				/*--------------------------+
				| constructors & destructor |
				+--------------------------*/

				public:
				Initializer() :
					origCoutBuf(std::cout.rdbuf()),
					origCerrBuf(std::cerr.rdbuf()),
					origClogBuf(std::clog.rdbuf())
				{
					/*-------------+
					| attach sinks |
					+-------------*/

					auto consoleSink(std::make_shared<ConsoleSink>(PACKAGE_NAME " log"));
					outStream = std::make_shared<BranchableStream>(consoleSink),
					errStream = std::make_shared<BranchableStream>(consoleSink);

					/*---------------------------------------------------+
					| replace std::streambuf for standard output streams |
					+---------------------------------------------------*/

					try
					{
						std::cout.rdbuf(&outStream->streambuf());
						std::cerr.rdbuf(&errStream->streambuf());
						std::clog.rdbuf(&errStream->streambuf());
					}
					catch (...)
					{
						std::cout.rdbuf(origCoutBuf);
						std::cerr.rdbuf(origCerrBuf);
						std::clog.rdbuf(origClogBuf);
						throw;
					}
				}

				/*-----------+
				| destructor |
				+-----------*/

				~Initializer()
				{
					std::cout.flush();
					std::clog.flush();

					std::cout.rdbuf(origCoutBuf);
					std::cerr.rdbuf(origCerrBuf);
					std::clog.rdbuf(origClogBuf);
				}

				/*-------------+
				| data members |
				+-------------*/

				private:
				std::shared_ptr<Stream>
					outStream,
					errStream;

				std::streambuf
					*const origCoutBuf,
					*const origCerrBuf,
					*const origClogBuf;

				std::ios_base::Init iostreamInitializer;

			} initializer __attribute__((init_priority(LOG_INIT_PRIORITY)));
		}
	}
}
