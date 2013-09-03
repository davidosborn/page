#include <iostream> // c{err,log,out}, ios_base::Init, streambuf
#include <memory> // shared_ptr

#include "../cfg/vars.hpp"
#include "../util/cpp.hpp" // STRINGIZE
#include "../util/gcc/init_priority.hpp" // LOG_INIT_PRIORITY
#include "filter/IndentFilter.hpp"
#include "filter/TimeFilter.hpp"
#include "sink/ConsoleSink.hpp"
#include "sink/FileSink.hpp"
#include "sink/StderrSink.hpp"
#include "sink/StdoutSink.hpp"
#include "stream/BufferStream.hpp"

namespace page
{
	namespace log
	{
		namespace
		{
			class Initializer
			{
				/*-------------+
				| constructors |
				+-------------*/

				public:
				Initializer() :
					origCoutBuf(std::cout.rdbuf()),
					origCerrBuf(std::cerr.rdbuf()),
					origClogBuf(std::clog.rdbuf())
				{
					/*-------------+
					| attach sinks |
					+-------------*/
					{
						auto
							outRootSink = std::make_shared<BranchableStream>(),
							errRootSink = std::make_shared<BranchableStream>();

						if (*CVAR(logConsole))
						{
							if (*CVAR(logConsoleSpawn))
							{
								auto consoleSink(std::make_shared<ConsoleSink>(STRINGIZE(PACKAGE) " log"));
								outRootSink->Attach(consoleSink);
								errRootSink->Attach(consoleSink);
							}
							else
							{
								outRootSink->Attach(std::make_shared<StdoutSink>());
								errRootSink->Attach(std::make_shared<StderrSink>());
							}
						}
						if (*CVAR(logFile))
						{
							auto fileSink(std::make_shared<FileSink>(*CVAR(logFilePath)));
							outRootSink->Attach(fileSink);
							errRootSink->Attach(fileSink);
						}

						outStream = outRootSink;
						errStream = errRootSink;
					}

					/*---------------------------------------+
					| build filter chains (from end to root) |
					+---------------------------------------*/

					outStream = std::make_shared<BufferStream>(outStream);
					logStream = std::make_shared<BufferStream>(errStream);

					outStream = std::make_shared<TimeFilter>(outStream);
					errStream = std::make_shared<TimeFilter>(errStream);
					logStream = std::make_shared<TimeFilter>(logStream);

					outStream = std::make_shared<IndentFilter>(outStream);
					errStream = std::make_shared<IndentFilter>(errStream);
					logStream = std::make_shared<IndentFilter>(logStream);

					/*---------------------------------------------------+
					| replace std::streambuf for standard output streams |
					+---------------------------------------------------*/

					try
					{
						std::cout.rdbuf(&outStream->streambuf());
						std::cerr.rdbuf(&errStream->streambuf());
						std::clog.rdbuf(&logStream->streambuf());
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
					errStream,
					logStream;

				std::streambuf
					*const origCoutBuf,
					*const origCerrBuf,
					*const origClogBuf;

				std::ios_base::Init iostreamInitializer;

			} initializer __attribute__((init_priority(LOG_INIT_PRIORITY)));
		}
	}
}
