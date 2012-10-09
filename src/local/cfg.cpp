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

#include <algorithm> // max, min
#include <iomanip> // setw
#include <iostream> // c{log,out}
#include <memory> // shared_ptr
#include <string>
#include <vector>
#include "cfg.hpp" // Res{Filters,Sources}
#include "cfg/Source.hpp" // Make{File,Site,User}Source, Source
#include "cfg/source/OptSource.hpp"
#include "cfg/Var.hpp"
#include "err/exception/catch.hpp" // CATCH_TAGS
#include "log/Indenter.hpp"
#include "log/manip.hpp" // Warn
#include "math/Vector.hpp"
#include "opt.hpp" // cfgSources
#include "util/ios.hpp" // BasicIosFormatSaver
#include "util/scoped_ptr.hpp"

namespace page
{
	namespace cfg
	{
		namespace
		{
			// configuration variable filters
			float LimitClipFrameRate(float value)
			{
				return std::max(value, 1.f);
			}
			float LimitClipQuality(float value)
			{
				return std::min(std::max(value, 0.f), 1.f);
			}
			float ConvertInClipQuality(int value)
			{
				return value / 100;
			}
			int ConvertOutClipQuality(float value)
			{
				return value * 100;
			}
		}

		// configuration variables
		Var<float>                     audVolume       ("aud.volume");
		Var<std::string>               clipFormat      ("clip.format");
		Var<float>                     clipFrameRate   ("clip.framerate", 30, LimitClipFrameRate);
		Var<std::string>               clipName        ("clip.name", "clip-%i");
		Var<float, int>                clipQuality     ("clip.quality", .75, LimitClipQuality, ConvertInClipQuality, ConvertOutClipQuality);
		Var<math::Vector<2, unsigned>> clipSize        ("clip.size", math::Vector<2, unsigned>(480, 360));
		Var<bool>                      dbgDrawBounds   ("dbg.draw.bounds");
		Var<bool>                      dbgDrawCollision("dbg.draw.collision");
		Var<bool>                      dbgDrawFps      ("dbg.draw.fps");
		Var<bool>                      dbgDrawSkeleton ("dbg.draw.skeleton");
		Var<bool>                      dbgDrawTrack    ("dbg.draw.track");
		Var<std::string>               installPath     ("install.path");
		Var<bool>                      logCache        ("log.cache");
		Var<bool>                      logCacheUpdate  ("log.cache.update");
		Var<bool>                      logCon          ("log.con");
		Var<bool>                      logConSpawn     ("log.con.spawn");
		Var<bool>                      logFile         ("log.file");
		Var<bool>                      logFileAppend   ("log.file.append");
		Var<std::string>               logFileFormat   ("log.file.format");
		Var<std::string>               logFileName     ("log.file.name", "log");
		Var<bool>                      logHost         ("log.host");
		Var<bool>                      logStats        ("log.stats");
		Var<bool>                      logSync         ("log.sync");
		Var<bool>                      logTime         ("log.time");
		Var<bool>                      logVerbose      ("log.verbose");
		Var<ResFilters>                resFilters      ("res.filter");
		Var<ResSources>                resSources      ("res.source", std::vector<std::string>(1, "data"));
		Var<std::string>               shotFormat      ("shot.format");
		Var<std::string>               shotName        ("shot.name", "shot-%i");
		Var<math::Vector<2, unsigned>> shotSize        ("shot.size", math::Vector<2, unsigned>(800, 600));
		Var<unsigned>                  vidRefresh      ("vid.refresh");
		Var<math::Vector<2, unsigned>> vidResolution   ("vid.resolution", math::Vector<2, unsigned>(640, 480));
		Var<bool>                      wndFull         ("wnd.full");
		Var<bool>                      wndMax          ("wnd.max");
		Var<math::Vector<2, int>>      wndPosition     ("wnd.position", math::Vector<2, int>(120, 90));
		Var<math::Vector<2, unsigned>> wndSize         ("wnd.size", math::Vector<2, unsigned>(400, 300));

		namespace
		{
			util::scoped_ptr<Source> siteSource, userSource;
			typedef std::vector<std::shared_ptr<Source>> AuxSources;
			AuxSources auxSources;

			bool Update(BasicVar &var, Reader &reader, Domain domain)
			{
				try
				{
					return var.Update(reader, domain);
				}
				CATCH_TAGS(err::DeserializationTag)
				{
					std::clog << log::Warn << "failed to deserialize " <<
						var.GetName() << " from " << reader.source.GetName() << std::endl;
				}
				catch (...)
				{
					std::clog << log::Warn << "failed to read " <<
						var.GetName() << " from " << reader.source.GetName() << std::endl;
				}
				return false;
			}
			void Commit(const BasicVar &var, Writer &writer, Domain domain)
			{
				try
				{
					var.Commit(writer, domain);
				}
				CATCH_TAGS(err::SerializationTag)
				{
					std::clog << log::Warn << "failed to serialize " <<
						var.GetName() << " to " << writer.source.GetName() << std::endl;
				}
				catch (...)
				{
					std::clog << log::Warn << "failed to write " <<
						var.GetName() << " to " << writer.source.GetName() << std::endl;
				}
			}
		}

		void Update();

		void Init()
		{
			try
			{
				siteSource.reset(MakeSiteSource());
			}
			catch (...)
			{
				std::clog << log::Warn << "failed to initialize site configuration" << std::endl;
			}
			try
			{
				userSource.reset(MakeUserSource());
			}
			catch (...)
			{
				std::clog << log::Warn << "failed to initialize user configuration" << std::endl;
			}
			for (opt::CfgSources::const_iterator source(opt::cfgSources.begin()); source != opt::cfgSources.end(); ++source)
			{
				try
				{
					auxSources.push_back(std::shared_ptr<Source>(MakeFileSource(*source)));
				}
				catch (...)
				{
					std::clog << log::Warn << "failed to load source: " << *source << std::endl;
				}
			}
			auxSources.push_back(std::shared_ptr<Source>(new OptSource));
			Update();
		}

		void Update()
		{
			std::cout << "reading configuration" << std::endl;
			log::Indenter indenter;
			// calculate cvar alignment width
			std::streamsize width = 0;
			for (const auto &var : BasicVar::GetGlobalRange())
				width = std::max<std::streamsize>(var.GetName().size(), width);
			// set flags for trailing alignment
			util::BasicIosFormatSaver<char> iosFormatSaver(std::cout);
			std::cout.setf(std::ios_base::left, std::ios_base::adjustfield);
			std::cout.fill(' ');
			// open source readers
			typedef std::vector<std::shared_ptr<Reader>> AuxReaders;
			AuxReaders auxReaders;
			for (AuxSources::const_iterator source(auxSources.begin()); source != auxSources.end(); ++source)
				auxReaders.push_back(std::shared_ptr<Reader>((*source)->OpenReader()));
			util::scoped_ptr<Reader> siteReader, userReader;
			if (siteSource)
			{
				try
				{
					siteReader.reset(siteSource->OpenReader());
				}
				catch (...)
				{
					std::clog << log::Warn << "failed to read site configuration" << std::endl;
				}
			}
			if (userSource)
			{
				try
				{
					userReader.reset(userSource->OpenReader());
				}
				catch (...)
				{
					std::clog << log::Warn << "failed to read user configuration" << std::endl;
				}
			}
			// read cvars
			for (auto &var : BasicVar::GetGlobalRange())
			{
				if (siteReader) Update(var, *siteReader, siteDomain);
				if (userReader) Update(var, *userReader, userDomain);
				if (!var.WasUserModified())
				{
					// read auxiliary sources into user domain
					for (AuxReaders::iterator reader(auxReaders.begin()); reader != auxReaders.end(); ++reader)
						Update(var, **reader, userDomain);
				}
				// print cvar value
				std::cout << std::setw(width) << var.GetName() << " = " << var.GetRepr() << std::endl;
			}
		}
		void Commit()
		{
			std::cout << "writing configuration" << std::endl;
			log::Indenter indenter;
			util::scoped_ptr<Writer> siteWriter, userWriter;
			if (siteSource)
			{
				try
				{
					siteWriter.reset(siteSource->OpenWriter());
				}
				catch (...)
				{
					std::clog << log::Warn << "failed to write site configuration" << std::endl;
				}
			}
			if (userSource)
			{
				try
				{
					userWriter.reset(userSource->OpenWriter());
				}
				catch (...)
				{
					std::clog << log::Warn << "failed to write user configuration" << std::endl;
				}
			}
			for (const auto &var : BasicVar::GetGlobalRange())
			{
				if (siteWriter) Commit(var, *siteWriter, siteDomain);
				if (userWriter) Commit(var, *userWriter, userDomain);
			}
		}
	}
}
