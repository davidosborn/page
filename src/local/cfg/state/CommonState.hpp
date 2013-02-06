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

#ifndef    page_local_cfg_state_CommonState_hpp
#   define page_local_cfg_state_CommonState_hpp

#	include <string>
#	include <vector>

	// Boost
#	include <boost/filesystem/path.hpp>
#	include <boost/regex.hpp>

	// local
#	include "../../math/Vector.hpp"
#	include "../var/Var.hpp"
#	include "BasicState.hpp"

namespace page
{
	namespace cfg
	{
		class CommonState : public virtual BasicState
		{
			/*--------------------------+
			| constructors & destructor |
			+--------------------------*/
			
			public:
			CommonState();
		
			/*------------------------+
			| configuration variables |
			+------------------------*/

			public:
			/**
			 * A configuration variable specifying the master audio volume in
			 * decibels.
			 */
			Var<float>                                   audioVolume;

			/**
			 * A configuration variable specifying the file path for in-game
			 * recordings.  If it is a relative path, it is interpreted as being
			 * relative to @c installPath.  The path will be expanded using
			 * @c util::ExpandPath.  The extension will be added automatically
			 * if its not included in the path.
			 */
			Var<boost::filesystem::path>                 clipFilePath;

			/**
			 * A configuration variable specifying the file format for in-game
			 * recordings.
			 */
			Var<std::string>                             clipFormat;

			/**
			 * A configuration variable specifying the frame rate for in-game
			 * recordings.
			 */
			Var<float>                                   clipFramerate;

			/**
			 * A configuration variable specifying the quality of in-game
			 * recordings.  Internally, it is a floating-point value ranging
			 * from 0 (lowest quality) to 1 (highest quality).  Externally, it
			 * is represented by a percentage.
			 */
			Var<float, int>                              clipQuality;

			/**
			 * A configuration variable specifying the video resolution for in-
			 * game recordings.
			 */
			Var<math::Vector<2, unsigned>>               clipVideoResolution;

			/**
			 * A configuration variable specifying whether to draw bounding
			 * boxes in debug mode.
			 */
			Var<bool>                                    debugDrawBounds;

			/**
			 * A configuration variable specifying whether to draw collision
			 * information in debug mode.
			 */
			Var<bool>                                    debugDrawCollision;

			/**
			 * A configuration variable specifying whether to draw the frame
			 * rate in debug mode.
			 */
			Var<bool>                                    debugDrawFramerate;

			/**
			 * A configuration variable specifying whether to draw skeletons in
			 * debug mode.
			 */
			Var<bool>                                    debugDrawSkeleton;

			/**
			 * A configuration variable specifying whether to draw the track in
			 * debug mode.
			 */
			Var<bool>                                    debugDrawTrack;

			/**
			 * A configuration variable specifying the installation path for the
			 * program.  This variable is used to resolve relative paths and to
			 * find files associated with the program.
			 */
			Var<boost::filesystem::path>                 installPath;

			/**
			 * A configuration variable specifying whether to include cache
			 * events in the log.
			 */
			Var<bool>                                    logCache;

			/**
			 * A configuration variable specifying whether to include cache
			 * update events in the log.
			 */
			Var<bool>                                    logCacheUpdate;

			/**
			 * A configuration variable specifying whether to write log output
			 * to the console.
			 */
			Var<bool>                                    logConsole;

			/**
			 * A configuration variable specifying whether to create (or attach)
			 * a console window.  Otherwise, the log output will simply be
			 * written to the existing standard I/O devices.
			 */
			Var<bool>                                    logConsoleSpawn;

			/**
			 * A configuration variable specifying whether to write log output
			 * to a file.
			 */
			Var<bool>                                    logFile;

			/**
			 * A configuration variable specifying whether to append log output
			 * to the file or overwrite the file each time the program is run.
			 */
			Var<bool>                                    logFileAppend;

			/**
			 * A configuration variable specifying the path of the log file. If
			 * it is a relative path, it is interpreted as being relative to
			 * @c installPath.
			 */
			Var<boost::filesystem::path>                 logFilePath;

			/**
			 * A configuration variable specifying whether to include the
			 * computer's hostname in the log.  This variable can be switched
			 * off for privacy when sending the log file to third parties.
			 */
			Var<bool>                                    logHostname;

			/**
			 * A configuration variable specifying whether to include statistics
			 * in the log.
			 */
			Var<bool>                                    logStats;

			/**
			 * A configuration variable specifying whether to synchronize
			 * @c std::cout and @c std::cerr.
			 */
			Var<bool>                                    logSync;

			/**
			 * A configuration variable specifying whether to include the time
			 * for each event logged.
			 */
			Var<bool>                                    logTime;

			/**
			 * A configuration variable specifying whether to only include the
			 * time when it has changed.
			 */
			Var<bool>                                    logTimeChange;

			/**
			 * A configuration variable specifying whether to include verbose
			 * information in the log.
			 */
			Var<bool>                                    logVerbose;

			/**
			 * A configuration variable specifying a list of regular expressions
			 * for filtering out resources by their path.
			 */
			Var<std::vector<boost::regex>>               resourceExcludes;

			/**
			 * A configuration variable specifying a list of resource sources.
			 */
			Var<std::vector<boost::filesystem::path>>    resourceSources;

			/**
			 * A configuration variable specifying the file path for screenshots.
			 * If it is a relative path, it is interpreted as being relative to
			 * @c installPath.  The path will be expanded using
			 * @c util::ExpandPath. The extension will be added automatically if
			 * its not included in the path.
			 */
			Var<boost::filesystem::path>                 screenshotFilePath;

			/**
			 * A configuration variable specifying the file format for
			 * screenshots.
			 */
			Var<std::string>                             screenshotFormat;

			/**
			 * A configuration variable specifying the video resolution for
			 * screenshots.
			 */
			Var<math::Vector<2, unsigned>>               screenshotSize;

			/**
			 * A configuration variable specifying the video refresh rate.  A
			 * value of 0 means to use the default refresh rate.
			 */
			Var<unsigned>                                videoRefresh;

			/**
			 * A configuration variable specifying the video resolution when the
			 * main window is in fullscreen mode.
			 */
			Var<math::Vector<2, unsigned>>               videoResolution;

			/**
			 * A configuration variable specifying whether the main window
			 * should start in fullscreen mode.
			 */
			Var<bool>                                    windowFullscreen;

			/**
			 * A configuration variable specifying whether the main window
			 * should start in maximized mode.
			 */
			Var<bool>                                    windowMaximized;

			/**
			 * A configuration variable specifying the initial position of the
			 * main window.
			 */
			Var<math::Vector<2, int>>                    windowPosition;

			/**
			 * A configuration variable specifying the initial size of the main
			 * window.
			 */
			Var<math::Vector<2, unsigned>>               windowSize;
		};
	}
}

#endif
