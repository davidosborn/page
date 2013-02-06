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

#ifndef    page_local_clip_encoder_registry_hpp
#   define page_local_clip_encoder_registry_hpp

	// Boost
#	include <boost/filesystem/path.hpp>

	// local
#	include <functional> // function
#	include <string>
#	include <utility> // pair
#	include "function.hpp" // EncoderFactory

namespace page
{
	namespace clip
	{
		/*-------+
		| access |
		+-------*/

		/**
		 * Search for a registered encoder that matches the given parameters and
		 * and return a @c std::pair containing a factory function to produce
		 * the encoder object and the path with the proper extension for the
		 * encoded format.
		 */
		std::pair<EncoderFactory, boost::filesystem::path>
			GetRegisteredEncoder(
				const boost::filesystem::path &path,
				const std::string &format);

		/*-------------+
		| registration |
		+-------------*/

		/**
		 * Register an encoder with the system.
		 */
		void RegisterEncoder(
			const EncoderFactory &,
			const std::string &extension,
			const std::string &format,
			unsigned rank);
	}
}

#endif
