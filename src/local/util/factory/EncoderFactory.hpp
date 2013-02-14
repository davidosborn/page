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

#ifndef    page_local_util_factory_EncoderFactory_hpp
#   define page_local_util_factory_EncoderFactory_hpp

	// C++
#	include <string>
#	include <unordered_set>

	// local
#	include "Factory.hpp"

namespace page
{
	namespace util
	{
		/**
		 *
		 */
		struct EncoderCriteria
		{
			std::unordered_set<std::string> formats;
			std::unordered_set<std::string> mimeTypes;
			std::unordered_set<std::string> extensions;

			using FormatCriterion    = MemberContainsCriterion<decltype(formats),    EncoderCriteria, &EncoderCriteria::formats>;
			using MimeTypeCriterion  = MemberContainsCriterion<decltype(mimeTypes),  EncoderCriteria, &EncoderCriteria::mimeTypes>;
			using ExtensionCriterion = MemberContainsCriterion<decltype(extensions), EncoderCriteria, &EncoderCriteria::extensions>;
		};
		
		/**
		 *
		 */
		struct EncoderData
		{
			/**
			 * The default extension, which can be appended to the output path
			 * if it doesn't already have an appropriate extension.
			 */
			std::string defaultExtension;
		};

		/**
		 * A factory for producing encoders according to a defined criteria.
		 */
		template <typename AbstractEncoder, typename ConstructorArgs>
			using EncoderFactory = Factory<AbstractEncoder, ConstructorArgs, EncoderCriteria, EncoderData>;
	}
}

#endif
