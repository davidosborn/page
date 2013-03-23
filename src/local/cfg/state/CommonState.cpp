/**
 * @section license
 *
 * Copyright (c) 2006-2013 David Osborn
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

#include <algorithm> // max, min
#include <functional> // bind

#include "../../util/cpp.hpp" // STRINGIZE
#include "../../util/path/filesystem.hpp" // AbsolutePath
#include "CommonState.hpp"

#define LOG_HOSTNAME_DEFAULT false

#ifndef NDEBUG
#	define LOG_CONSOLE_DEFAULT  true
#	define LOG_FILE_DEFAULT     true
#	define LOG_STATS_DEFAULT    true
#	define LOG_VERBOSE_DEFAULT  true
#else
#	define LOG_CONSOLE_DEFAULT  false
#	define LOG_FILE_DEFAULT     false
#	define LOG_STATS_DEFAULT    false
#	define LOG_VERBOSE_DEFAULT  false
#endif

namespace page { namespace cfg
{
	namespace
	{
		/*-----------------------+
		| clip.file.path filters |
		+-----------------------*/

		std::string GetClipFilePath(const std::string &path, const Var<std::string> &installPath)
		{
			return util::AbsolutePath(path, *installPath);
		}

		/*-----------------------+
		| clip.framerate filters |
		+-----------------------*/

		/**
		 * Limits @c clip.framerate to a reasonable range.
		 */
		float SetClipFrameRate(float value)
		{
			return std::min(std::max(value, 1.f), 120.f);
		}

		/*---------------------+
		| clip.quality filters |
		+---------------------*/

		/**
		 * Limits @c clip.quality to a unit ratio.
		 */
		float SetClipQuality(float value)
		{
			return std::min(std::max(value, 0.f), 1.f);
		}

		/**
		 * Converts @c clip.quality from a percentage to a unit ratio.
		 */
		float ConvertInClipQuality(int value)
		{
			return value / 100.f;
		}

		/**
		 * Converts @c clip.quality from a unit ratio to a percentage.
		 */
		int ConvertOutClipQuality(float value)
		{
			return value * 100;
		}

		/*---------------------+
		| install.path filters |
		+---------------------*/

		/**
		 * Returns @c install.path as an absolute path.
		 */
		std::string GetInstallPath(const std::string &path)
		{
			return util::AbsolutePath(path);
		}

		/*----------------------+
		| log.file.path filters |
		+----------------------*/

		/**
		 * Returns @c log.file.path as an absolute path.
		 */
		std::string GetLogFilePath(const std::string &path, const Var<std::string> &installPath)
		{
			return util::AbsolutePath(path, *installPath);
		}

		/*-----------------------------+
		| screenshot.file.path filters |
		+-----------------------------*/

		std::string GetScreenshotFilePath(const std::string &path, const Var<std::string> &installPath)
		{
			return util::AbsolutePath(path, *installPath);
		}
	}

	/*--------------------------+
	| constructors & destructor |
	+--------------------------*/

	CommonState::CommonState() :
		audioVolume        (*this, "audio.volume",          1),
		clipFilePath       (*this, "clip.file.path",        "clip-%i",                 std::bind(GetClipFilePath, std::placeholders::_1, installPath)),
		clipFormat         (*this, "clip.format",           ""),
		clipFramerate      (*this, "clip.framerate",        30,                        nullptr, SetClipFrameRate),
		clipQuality        (*this, "clip.quality",          .75f,                      nullptr, SetClipQuality, ConvertInClipQuality, ConvertOutClipQuality),
		clipVideoResolution(*this, "clip.video.resolution", {480, 360}),
		debugDrawBounds    (*this, "debug.draw.bounds",     false),
		debugDrawCollision (*this, "debug.draw.collision",  false),
		debugDrawFramerate (*this, "debug.draw.framerate",  false),
		debugDrawSkeleton  (*this, "debug.draw.skeleton",   false),
		debugDrawTrack     (*this, "debug.draw.track",      false),
		installPath        (*this, "install.path",          "",                        GetInstallPath),
		logCache           (*this, "log.cache",             false),
		logCacheUpdate     (*this, "log.cache.update",      false),
		logConsole         (*this, "log.console",           LOG_CONSOLE_DEFAULT),
		logConsoleSpawn    (*this, "log.console.spawn",     true),
		logFile            (*this, "log.file",              LOG_FILE_DEFAULT),
		logFileAppend      (*this, "log.file.append",       false),
		logFilePath        (*this, "log.file.path",         STRINGIZE(PACKAGE) ".log", std::bind(GetLogFilePath, std::placeholders::_1, installPath)),
		logHostname        (*this, "log.hostname",          LOG_HOSTNAME_DEFAULT),
		logStats           (*this, "log.stats",             LOG_STATS_DEFAULT),
		logSync            (*this, "log.sync",              false),
		logTime            (*this, "log.time",              true),
		logTimeChange      (*this, "log.time.change",       true),
		logVerbose         (*this, "log.verbose",           LOG_VERBOSE_DEFAULT),
		resourceExcludes   (*this, "resource.excludes",     {}),
		resourceSources    (*this, "resource.sources",      {"data"}),
		screenshotFilePath (*this, "screenshot.file.path",  "screenshot-%i",           std::bind(GetScreenshotFilePath, std::placeholders::_1, installPath)),
		screenshotFormat   (*this, "screenshot.format",     ""),
		screenshotSize     (*this, "screenshot.size",       {800, 600}),
		videoRefresh       (*this, "video.refresh",         0),
		videoResolution    (*this, "video.resolution",      {640, 480}),
		windowFullscreen   (*this, "window.fullscreen",     false),
		windowMaximized    (*this, "window.maximized",      false),
		windowPosition     (*this, "window.position",       {120, 90}),
		windowSize         (*this, "window.size",           {400, 300}) {}
}}
