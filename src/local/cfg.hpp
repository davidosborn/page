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

#ifndef    page_cfg_hpp
#   define page_cfg_hpp

#	include <string>
#	include <vector>
#	include "cfg/Var.hpp"
#	include "math/fwd.hpp" // Vector

namespace page
{
	namespace cfg
	{
		typedef std::vector<std::string> ResFilters;
		typedef std::vector<std::string> ResSources;

		// configuration variables
		extern Var<float>                       audVolume;
		extern Var<std::string>                 clipFormat;
		extern Var<float>                       clipFrameRate;
		extern Var<std::string>                 clipName;
		extern Var<float, int>                  clipQuality;
		extern Var<math::Vector<2, unsigned>>   clipSize;
		extern Var<bool>                        dbgDrawBounds;
		extern Var<bool>                        dbgDrawCollision;
		extern Var<bool>                        dbgDrawFps;
		extern Var<bool>                        dbgDrawSkeleton;
		extern Var<bool>                        dbgDrawTrack;
		extern Var<std::string>                 installPath;
		extern Var<bool>                        logCache;
		extern Var<bool>                        logCacheUpdate;
		extern Var<bool>                        logCon;
		extern Var<bool>                        logConSpawn;
		extern Var<bool>                        logFile;
		extern Var<bool>                        logFileAppend;
		extern Var<std::string>                 logFileFormat;
		extern Var<std::string>                 logFileName;
		extern Var<bool>                        logHost;
		extern Var<bool>                        logStats;
		extern Var<bool>                        logSync;
		extern Var<bool>                        logTime;
		extern Var<bool>                        logVerbose;
		extern Var<ResFilters>                  resFilters;
		extern Var<ResSources>                  resSources;
		extern Var<std::string>                 shotFormat;
		extern Var<std::string>                 shotName;
		extern Var<math::Vector<2, unsigned>>   shotSize;
		extern Var<unsigned>                    vidRefresh;
		extern Var<math::Vector<2, unsigned>>   vidResolution;
		extern Var<bool>                        wndFull;
		extern Var<bool>                        wndMax;
		extern Var<math::Vector<2, int>>        wndPosition;
		extern Var<math::Vector<2, unsigned>>   wndSize;

		void Init();

		void Update();
		void Commit();
	}
}

#endif
