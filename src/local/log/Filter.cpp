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

#include <cassert>
#include "Filter.hpp"

namespace page
{
	namespace log
	{
		Filter::Filter() {}
		Filter::Filter(const std::shared_ptr<Stream> &link) : links(1, link)
		{
			assert(link);
		}

		void Filter::Attach(const std::shared_ptr<Stream> &link)
		{
			assert(link);
			links.push_back(link);
		}
		void Filter::Detach()
		{
			links.clear();
		}
		void Filter::Extend(Filter &other)
		{
			links.insert(links.end(), other.links.begin(), other.links.end());
		}

		void Filter::Put(char c)
		{
			for (std::vector<std::shared_ptr<Stream>>::iterator link = links.begin(); link != links.end(); ++link)
				(*link)->Put(c);
		}
		void Filter::Put(const char *s, unsigned n)
		{
			for (std::vector<std::shared_ptr<Stream>>::iterator link = links.begin(); link != links.end(); ++link)
				(*link)->Put(s, n);
		}

		void Filter::Sync()
		{
			for (std::vector<std::shared_ptr<Stream>>::iterator link = links.begin(); link != links.end(); ++link)
				(*link)->Sync();
		}

		void Filter::Clear()
		{
			for (std::vector<std::shared_ptr<Stream>>::iterator link = links.begin(); link != links.end(); ++link)
				(*link)->Clear();
		}
		void Filter::Reset()
		{
			for (std::vector<std::shared_ptr<Stream>>::iterator link = links.begin(); link != links.end(); ++link)
				(*link)->Reset();
		}
	}
}
