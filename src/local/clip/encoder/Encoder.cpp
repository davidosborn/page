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

// C++
#include <iostream> // clog

// local
#include "../../err/Exception.hpp"
#include "../../log/manip.hpp" // Warning
#include "../../util/path.hpp" // {Get,With}Extension
#include "Encoder.hpp"

namespace page
{
	namespace clip
	{
////////// detail::EncoderFactory //////////////////////////////////////////////

		namespace detail
		{
			const typename EncoderFactory::Blueprint &
				EncoderFactory::SelectBest(
					boost::filesystem::path &path,
					const std::string &format) const
			{
				auto extension(util::GetExtension(path));

				const Encoder::Factory::Blueprint *blueprint(nullptr);

				// try to match format
				if (!format.empty())
				{
					try
					{
						blueprint = &GLOBAL(Encoder::Factory).SelectBest(
							EncoderFactory::Criteria::FormatCriterion(format));
						goto Selected;
					}
					catch (const err::Exception<err::FactoryTag>::Permutation &)
					{
						std::clog << log::Warning << "format '" << format << "' does not match any registered encoders" << std::endl;
					}
				}

				// try to match extension
				{
					std::string extension(util::GetExtension(path));
					if (!extension.empty())
					{
						try
						{
							blueprint = &GLOBAL(Encoder::Factory).SelectBest(
								EncoderFactory::Criteria::ExtensionCriterion(extension));
							goto Selected;
						}
						catch (const err::Exception<err::FactoryTag>::Permutation &)
						{
							std::clog << log::Warning << "extension '" << extension << "' does not match any registered encoders" << std::endl;
						}
					}
				}

				blueprint = &GLOBAL(Encoder::Factory).SelectBest();

				Selected:
				path = util::WithExtension(path,
					blueprint->criteria.extensions.begin(),
					blueprint->criteria.extensions.end());

				return *blueprint;
			}
		}

////////// Encoder /////////////////////////////////////////////////////////////

		/*--------------------------+
		| constructors & destructor |
		+--------------------------*/

		Encoder::Encoder(const Callback &cb, unsigned frameSize) :
			cb(cb), frameSize(frameSize) {}

		/*----------+
		| raw input |
		+----------*/

		void Encoder::Write(const void *s, unsigned n)
		{
			while (n)
			{
				unsigned n2 = std::min(n, frameSize - buffer.size());
				buffer.insert(buffer.end(),
					static_cast<const char *>(s),
					static_cast<const char *>(s) + n2);
				s = static_cast<const char *>(s) + n2;
				n -= n2;
				if (buffer.size() == frameSize)
				{
					Encode(&*buffer.begin());
					buffer.clear();
				}
			}
		}

		/*---------------+
		| encoded output |
		+---------------*/

		void Encoder::Return(const void *s, unsigned n) const
		{
			cb(s, n);
		}
	}
}
