/**
 * @copyright
 *
 * Copyright (c) 2006-2014 David Osborn
 *
 * Permission is granted to use and redistribute this software in source and
 * binary form, with or without modification, subject to the following
 * conditions:
 *
 * 1. Redistributions in source form must retain the above copyright notice,
 *    this list of conditions, and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions, and the following disclaimer in the same place
 *    and form as other copyright, license, and disclaimer information.
 *
 * 3. Redistributions in binary form must also include an acknowledgement in the
 *    same place and form as other acknowledgements (such as the credits),
 *    similar in substance to the following:
 *
 *       Portions of this software are based on the work of David Osborn.
 *
 * This software is provided "as is", without any express or implied warranty.
 * In no event will the authors be liable for any damages arising out of the use
 * of this software.
 */

#include <string>

#include <boost/filesystem/path.hpp>

namespace page { namespace util
{
	std::string AbsolutePath(
		std::string const& path,
		std::string const& root)
	{
		// FIXME: implement
	}

	std::string Basename(const std::string &path)
	{
		return path.substr(path.rfind('/') + 1);
	}

	std::string Dirname(const std::string &path)
	{
		auto sep(path.rfind('/'));
		return sep != std::string::npos ? path.substr(0, sep) : "";
	}

	std::string PrettyPath(const std::string &path)
	{
		// FIXME: implement
	}

	std::string RelativePath(
		std::string const& path,
		std::string const& root)
	{
		// FIXME: implement
	}

	std::string UrlPath(
		std::string const& path,
		std::string const& root)
	{
		// FIXME: implement
	}
}}
