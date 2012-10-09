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

#include <ctime> // localtime, time
#include <iostream> // cerr, clog, cout, streambuf
#include <locale> // locale, use_facet, time_put
#include <memory> // shared_ptr
#include "cfg.hpp" // log*
#include "log/filter/BufferFilter.hpp"
#include "log/filter/IndentFilter.hpp"
#include "log/filter/TimeFilter.hpp"
#include "log/Indenter.hpp"
#include "log/sink/BufferSink.hpp"
#include "log/sink/ConSink.hpp"
#include "log/sink/ErrSink.hpp"
#include "log/sink/FileSink.hpp"
#include "log/sink/OutSink.hpp"
#include "sys/file.hpp" // AbsPath, NormPath
#include "util/ios.hpp" // InitIos
#include "util/locale.hpp" // InitLocale
#include "util/path.hpp" // ExpandPath
#include "util/pp.hpp" // STRINGIZE
#include "util/scoped_ptr.hpp"

namespace page
{
	namespace log
	{
		namespace
		{
			std::streambuf *coutBuf, *cerrBuf, *clogBuf;
			Filter *lastOutFilter, *lastErrFilter;
		}

		void Preinit()
		{
			// initialize default locale and stream state
			util::InitLocale();
			std::cout.imbue(std::locale()); util::InitIos(std::cout);
			std::cerr.imbue(std::locale()); util::InitIos(std::cerr);
			std::clog.imbue(std::locale()); util::InitIos(std::clog);
			// FIXME: this inhibits console output for some reason
//			std::ios::sync_with_stdio(false);
			// build transitional stream chain with buffer sink
			std::shared_ptr<Stream>
				outBufferSink(new BufferSink(false)),
				errBufferSink(new BufferSink(true)),
				outTimeFilter(lastOutFilter = new TimeFilter(outBufferSink)),
				errTimeFilter(lastErrFilter = new TimeFilter(errBufferSink)),
				outIndentFilter(new IndentFilter(outTimeFilter)),
				errIndentFilter(new IndentFilter(errTimeFilter));
			util::scoped_ptr<Stream>
				outBufferFilter(new BufferFilter(outIndentFilter)),
				errBufferFilter(new BufferFilter(errIndentFilter));
			coutBuf = std::cout.rdbuf();
			cerrBuf = std::cerr.rdbuf();
			clogBuf = std::clog.rdbuf();
			try
			{
				std::cout.rdbuf(outBufferFilter.get()); outBufferFilter.release();
				std::cerr.rdbuf(errBufferFilter.get()); errBufferFilter.release();
				std::clog.rdbuf(std::cerr.rdbuf());
				// print current date/time
				std::time_t time = std::time(0);
				std::cout << "execution initiated on ";
				const char timePattern[] = "%x at %X";
				std::use_facet<std::time_put<char>>(std::cout.getloc()).put(
					std::cout, std::cout, std::cout.fill(), std::localtime(&time),
					timePattern, timePattern + sizeof timePattern - 1);
				std::cout << std::endl;
				// print program information
				std::cout << "this is " STRINGIZE(NAME) " " STRINGIZE(VERSION) ": " STRINGIZE(LONGNAME) << std::endl;
				log::Indenter indenter;
				std::cout << "built on " STRINGIZE(BUILD_DATE) " at " STRINGIZE(BUILD_TIME) << std::endl;
#if HOST != BUILD
				std::cout << "cross-compiled under " STRINGIZE(BUILD) " for " STRINGIZE(HOST) << std::endl;
#endif
			}
			catch (...)
			{
				std::clog.rdbuf(clogBuf);
				std::cerr.rdbuf(cerrBuf);
				std::cout.rdbuf(coutBuf);
				throw;
			}
		}
		void Init()
		{
			// build permanent stream chain
			BufferFilter outBufferFilter, errBufferFilter;
			if (*cfg::logCon)
			{
				if (*cfg::logConSpawn)
				{
					std::shared_ptr<Stream> conSink(new ConSink(STRINGIZE(NAME) " log"));
					outBufferFilter.Attach(conSink);
					errBufferFilter.Attach(conSink);
				}
				else
				{
					outBufferFilter.Attach(std::shared_ptr<Stream>(new OutSink));
					errBufferFilter.Attach(std::shared_ptr<Stream>(new ErrSink));
				}
			}
			if (*cfg::logFile)
			{
				std::shared_ptr<Stream> fileSink(new FileSink(
					sys::AbsPath(sys::NormPath(util::ExpandPath(*cfg::logFileName)))));
				outBufferFilter.Attach(fileSink);
				errBufferFilter.Attach(fileSink);
			}
			// switch to permanent stream chain
			std::cout.flush();
			std::clog.flush();
			BufferSink::Flush(outBufferFilter, errBufferFilter);
			lastOutFilter->Detach(); lastOutFilter->Extend(outBufferFilter);
			lastErrFilter->Detach(); lastErrFilter->Extend(errBufferFilter);
		}
		void Deinit()
		{
			// print statistics
			if (*cfg::logStats)
			{
				// FIXME: implement
			}
			// flush buffered streams
			std::cout.flush();
			std::clog.flush();
			// reset default stream buffers
			std::clog.rdbuf(clogBuf);
			delete std::cerr.rdbuf(cerrBuf);
			delete std::cout.rdbuf(coutBuf);
			// write remaining buffered output from preinit stage
			// NOTE: output will be written to default stream buffers
			OutSink outSink;
			ErrSink errSink;
			BufferSink::Flush(outSink, errSink);
		}
	}
}
