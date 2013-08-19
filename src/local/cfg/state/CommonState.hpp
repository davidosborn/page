#ifndef    page_local_cfg_state_CommonState_hpp
#   define page_local_cfg_state_CommonState_hpp

#	include <string>
#	include <vector>

#	include <boost/regex.hpp>

#	include "../../math/Vector.hpp"
#	include "../var/Var.hpp"
#	include "BasicState.hpp"

namespace page { namespace cfg
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

		/**
		 * A configuration variable specifying the master audio volume in
		 * decibels.
		 */
		Var<float>                                   audioVolume;

		/**
		 * A configuration variable specifying the file path for in-game
		 * recordings.  If it is a relative path, it is interpreted as being
		 * relative to @c installPath.  The path will be expanded using
		 * @c util::ExpandPath.  The extension will be added automatically if
		 * it is not included in the path.
		 */
		Var<std::string>                             clipFilePath;

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
		 * recordings.  Internally, it is a floating-point value ranging from 0
		 * (lowest quality) to 1 (highest quality).  Externally, it is
		 * represented by a percentage.
		 */
		Var<float, int>                              clipQuality;

		/**
		 * A configuration variable specifying the video resolution for in-game
		 * recordings.
		 */
		Var<math::Vec2u>                             clipVideoResolution;

		/**
		 * A configuration variable specifying whether to draw bounding boxes in
		 * debug mode.
		 */
		Var<bool>                                    debugDrawBounds;

		/**
		 * A configuration variable specifying whether to draw collision
		 * information in debug mode.
		 */
		Var<bool>                                    debugDrawCollision;

		/**
		 * A configuration variable specifying whether to draw the frame rate in
		 * debug mode.
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
		 * program.  This variable is used to resolve relative paths and to find
		 * files associated with the program.
		 */
		Var<std::string>                             installPath;

		/**
		 * A configuration variable specifying whether to include cache events
		 * in the log.
		 */
		Var<bool>                                    logCache;

		/**
		 * A configuration variable specifying whether to include cache update
		 * events in the log.
		 */
		Var<bool>                                    logCacheUpdate;

		/**
		 * A configuration variable specifying whether to write log output to
		 * the console.
		 */
		Var<bool>                                    logConsole;

		/**
		 * A configuration variable specifying whether to create (or attach) a
		 * console window.  Otherwise, the log output will simply be written to
		 * the existing standard I/O devices.
		 */
		Var<bool>                                    logConsoleSpawn;

		/**
		 * A configuration variable specifying whether to write log output to a
		 * file.
		 */
		Var<bool>                                    logFile;

		/**
		 * A configuration variable specifying whether to append log output to
		 * the file or overwrite the file each time the program is run.
		 */
		Var<bool>                                    logFileAppend;

		/**
		 * A configuration variable specifying the path of the log file. If it
		 * is a relative path, it is interpreted as being relative to
		 * @c installPath.
		 */
		Var<std::string>                             logFilePath;

		/**
		 * A configuration variable specifying whether to include the computer's
		 * hostname in the log.  This variable can be switched off for privacy
		 * when sending the log file to third parties.
		 */
		Var<bool>                                    logHostname;

		/**
		 * A configuration variable specifying whether to include statistics in
		 * the log.
		 */
		Var<bool>                                    logStats;

		/**
		 * A configuration variable specifying whether to synchronize
		 * @c std::cout and @c std::cerr.
		 */
		Var<bool>                                    logSync;

		/**
		 * A configuration variable specifying whether to include the time for
		 * each event logged.
		 */
		Var<bool>                                    logTime;

		/**
		 * A configuration variable specifying whether to only include the time
		 * when it has changed.
		 */
		Var<bool>                                    logTimeChange;

		/**
		 * A configuration variable specifying whether to include verbose
		 * information in the log.
		 */
		Var<bool>                                    logVerbose;

		/**
		 * A configuration variable specifying a list of regular expressions for
		 * filtering out resources by their path.
		 */
		Var<std::vector<boost::regex>>               resourceExcludes;

		/**
		 * A configuration variable specifying a list of resource sources.
		 */
		Var<std::vector<std::string>>                resourceSources;

		/**
		 * A configuration variable specifying the file path for screenshots.
		 * If it is a relative path, it is interpreted as being relative to
		 * @c installPath.  The path will be expanded using @c util::ExpandPath.
		 * The extension will be added automatically if its not included in the
		 * path.
		 */
		Var<std::string>                             screenshotFilePath;

		/**
		 * A configuration variable specifying the file format for screenshots.
		 */
		Var<std::string>                             screenshotFormat;

		/**
		 * A configuration variable specifying the video resolution for
		 * screenshots.
		 */
		Var<math::Vec2u>                             screenshotSize;

		/**
		 * A configuration variable specifying the video refresh rate.  A value
		 * of 0 means to use the default refresh rate.
		 */
		Var<unsigned>                                videoRefresh;

		/**
		 * A configuration variable specifying the video resolution when the
		 * main window is in fullscreen mode.
		 */
		Var<math::Vec2u>                             videoResolution;

		/**
		 * A configuration variable specifying whether the main window should
		 * start in fullscreen mode.
		 */
		Var<bool>                                    windowFullscreen;

		/**
		 * A configuration variable specifying whether the main window should
		 * start in maximized mode.
		 */
		Var<bool>                                    windowMaximized;

		/**
		 * A configuration variable specifying the initial position of the main
		 * window.
		 */
		Var<math::Vec2i>                             windowPosition;

		/**
		 * A configuration variable specifying the initial size of the main
		 * window.
		 */
		Var<math::Vec2u>                             windowSize;
	};
}}

#endif
