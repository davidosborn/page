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

#include <algorithm> // equal, find_if_not
#include <iostream> // clog

#include "../../../log/manip.hpp" // Warning
#include "../../../util/functional/locale.hpp" // isspace_function
#include "../../../util/string/operations.hpp" // Partition, Trim{Leading,Trailing}
#include "../../pipe/Stream.hpp" // Stream::GetLine
#include "LineIterator.hpp"

namespace page
{
	namespace res
	{
		LineIterator::LineIterator(Stream &stream, char comment) :
			stream(stream), comment(comment), indentation(0), warned(false) {}

		LineIterator::reference LineIterator::operator *() const
		{
			if (line.empty()) Fetch();
			return line;
		}
		LineIterator::pointer LineIterator::operator ->() const
		{
			if (line.empty()) Fetch();
			return &line;
		}

		LineIterator &LineIterator::operator ++()
		{
			if (line.empty()) stream.GetLine();
			else line.clear();
		}
		LineIterator LineIterator::operator ++(int)
		{
			if (line.empty()) stream.GetLine();
			else line.clear();
		}

		LineIterator::operator bool() const
		{
			return !line.empty() || (Fetch(), !line.empty());
		}

		unsigned LineIterator::Indentation() const
		{
			return indentation;
		}

		void LineIterator::Fetch()
		{
			do if (!(line = util::TrimTrailing(
				util::Partition(stream.GetLine(), comment).first)).empty())
			{
				indentation = 0;
				if (std::isspace(*line.begin()))
				{
					// determine whitespace format
					if (space.empty())
						space.assign(line.begin(), std::find_if_not(
						line.begin(), line.end(), util::isspace_function<char>()));
					// count indentation steps
					for (std::string::const_iterator iter(line.begin());
						line.size() - (iter - line.begin()) > space.size() &&
						std::equal(iter, iter + space.size(), space.begin());
						iter += space.size()) ++indentation;
					// verify indentation
					if (!warned && std::isspace(line[indentation * space.size()]))
					{
						std::clog << log::Warning << "inconsistent indentation" << std::endl;
						warned = true;
					}
					line = util::TrimLeading(line);
				}
				break;
			}
			while (!stream.Eof());
		}
		void LineIterator::Fetch() const
		{
			const_cast<LineIterator *>(this)->Fetch();
		}
	}
}
