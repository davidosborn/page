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

#ifndef    page_local_clip_encoder_EncoderFactory_hpp
#   define page_local_clip_encoder_EncoderFactory_hpp

	// C++
#	include <functional> // function
#	include <list>
#	include <unordered_map>
#	include <memory> // unique_ptr
#	include <string> // string
#	include <vector>

	// Boost
#	include <boost/filesystem/path.hpp>

#	include "../../util/factory/EncoderFactory.hpp"

namespace page
{
	namespace clip
	{
		class Callback;
		class Encoder;

		/**
		 * An factory for producing concrete instances of an abstract encoder
		 * type, where the concrete type is selected from a pool of registered
		 * encoders according to a standard criteria.
		 *
		 * @note Based in part on "Conversations: Abstract Factory, Template
		 *       Style" by Jim Hyslop and Herb Sutter.
		 */
		class EncoderFactory :
			public util::EncoderFactory<
				Encoder,
				util::ConstructorArgs<
					const Callback &,
					const math::Vec2u &,
					float,
					float>>
		{
			/*-----------+
			| production |
			+-----------*/

			public:
			/**
			 * Contains the information necessary to use a selected encoder.
			 */
			struct Product
			{
				const Function &function;
				boost::filesystem::path path;
			};

			/**
			 * Returns a new encoder instance.
			 *
			 * Tries the factory functions of encoders that match the specified
			 * criteria.  If none of the matching encoders work, tries the non-
			 * matching ones.
			 */
			Product Select(
				const boost::filesystem::path &,
				const std::vector<std::string> &formats,
				const std::vector<std::string> &mimeTypes) const;
		};
	}
}

#endif
