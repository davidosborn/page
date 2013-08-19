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
