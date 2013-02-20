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

#include <iostream> // clog

#include <boost/range/adaptor/transformed.hpp>

#include "../../../err/Exception.hpp"
#include "../../../log/manip.hpp" // Warning
#include "../../../util/functional/factory.hpp" // constructor_function
#include "../../../util/path/extension.hpp" // {Get,With}Extension
#include "EncoderFactory.hpp"

namespace page
{
	namespace clip
	{
		namespace detail
		{
			const typename EncoderFactory::Blueprint &
				EncoderFactory::SelectBest(
					boost::filesystem::path &path,
					const std::string &format) const
			{
				auto extension(util::GetExtension(path));

				const Blueprint *blueprint(nullptr);

				// try to match format
				if (!format.empty())
				{
					try
					{
						blueprint = &SelectBest(Criteria::FormatCriterion(format));
						goto Selected;
					}
					catch (const err::Exception<err::FactoryTag>::Permutation &)
					{
						std::clog << log::Warning << "format '" << format << "' does not match any registered encoders" << std::endl;
					}
				}

				// try to match extension
				{
					std::string extension(util::GetExtension(path).string());
					if (!extension.empty())
					{
						try
						{
							blueprint = &SelectBest(Criteria::ExtensionCriterion(extension));
							goto Selected;
						}
						catch (const err::Exception<err::FactoryTag>::Permutation &)
						{
							std::clog << log::Warning << "extension '" << extension << "' does not match any registered encoders" << std::endl;
						}
					}
				}

				blueprint = &SelectBest();

				Selected:
				path = util::WithExtension(path,
					boost::adaptors::transform(blueprint->criteria.extensions,
						util::constructor_function<boost::filesystem::path, const std::string &>()));

				return *blueprint;
			}
		}
	}
}
