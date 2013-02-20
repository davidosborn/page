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

#include <algorithm> // lower_bound
#include <cassert>
#include <vector>

#include "../../err/Exception.hpp"
#include "function.hpp" // SourceFactory

namespace page
{
	namespace res
	{
		namespace
		{
			struct Registry
			{
				struct Source
				{
					SourceFactory factory;
					unsigned rank;
				};
				typedef std::vector<Source> Sources;
				Sources sources;
			};
			inline Registry &GetRegistry()
			{
				static Registry reg;
				return reg;
			}
		}

		// creation
		Source *MakeSource(const std::string &path)
		{
			const Registry &reg(GetRegistry());
			for (Registry::Sources::const_iterator iter(reg.sources.begin()); iter != reg.sources.end(); ++iter)
			{
				Source *source = iter->factory(path);
				if (source) return source;
			}
			THROW((err::Exception<err::ResModuleTag, err::NotFoundTag>("source not found") <<
				boost::errinfo_file_name(path)))
		}

		// registration
		void RegisterSource(const SourceFactory &factory, unsigned rank)
		{
			assert(factory);
			Registry &reg(GetRegistry());
			Registry::Source source = {factory, rank};
			reg.sources.insert(
				std::lower_bound(
					reg.sources.begin(),
					reg.sources.end(),
					source,
					[](
						const Registry::Source &a,
						const Registry::Source &b) { return a.rank > b.rank; }),
				source);
		}
	}
}
