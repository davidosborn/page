#include <algorithm> // find_if_not
#include <cassert> // assert
#include <istream> // basic_istream
#include <iterator> // istreambuf_iterator
#include <sstream> // basic_istringstream
#include <string> // basic_string

namespace page
{
	namespace util
	{
		/*-----------------------+
		| implementation details |
		+-----------------------*/

		namespace detail
		{
			template <typename Char, typename CharTraits, typename T>
				std::basic_istream<Char, CharTraits> &Deserialize(
					std::basic_istream<Char, CharTraits> &is,
					T &value)
			{
				return is >> value;
			}

			template <typename Char, typename CharTraits, typename Allocator>
				std::basic_istream<Char, CharTraits> &Deserialize(
					std::basic_istream<Char, CharTraits> &is,
					std::basic_string<Char, CharTraits, Allocator> &s)
			{
				s.assign(
					std::istreambuf_iterator<Char>(is),
					std::istreambuf_iterator<Char>());
				return is;
			}
		}

		/*--------------------------+
		| constructors & destructor |
		+--------------------------*/

		template <typename T, typename Char, typename CharTraits>
			greedy_istream_iterator<T, Char, CharTraits>::
			greedy_istream_iterator() {}

		template <typename T, typename Char, typename CharTraits>
		template <typename Separator, typename Terminator>
			greedy_istream_iterator<T, Char, CharTraits>::
			greedy_istream_iterator(Stream &is, Separator separator, Terminator terminator) :
				is(&is), separator(separator), terminator(terminator)
		{
			++*this;
		}

		/*--------------------------------+
		| std::istream_iterator semantics |
		+--------------------------------*/

		template <typename T, typename Char, typename CharTraits>
			const T &
			greedy_istream_iterator<T, Char, CharTraits>::
			operator *() const
		{
			return value;
		}

		template <typename T, typename Char, typename CharTraits>
			const T *
			greedy_istream_iterator<T, Char, CharTraits>::
			operator ->() const
		{
			return &value;
		}

		template <typename T, typename Char, typename CharTraits>
			greedy_istream_iterator<T, Char, CharTraits> &
			greedy_istream_iterator<T, Char, CharTraits>::
			operator ++()
		{
			assert(is);

			if (terminated)
			{
				is = nullptr; // become end-of-stream iterator
				return *this;
			}

			typename std::basic_istream<Char, CharTraits>::sentry sentry(*is);
			if (!sentry) return *this;

			// read until next delimiter or end-of-stream
			std::basic_string<Char, CharTraits> s;
			for (;;)
			{
				auto c = is->peek();
				if (!CharTraits::not_eof(c))
				{
					terminated = true;
					break;
				}
				is->ignore();
				auto ch = CharTraits::to_char_type(c);
				if (terminator(ch))
				{
					terminated = true;
					break;
				}
				if (separator(ch))
					break;
				s.push_back(ch);
			}

			// remove trailing whitespace
			if (is->flags() & std::ios_base::skipws)
				s.erase(
					std::find_if_not(s.rbegin(), s.rend(),
						Delimiter::GetSpaceDelimiter()).base(),
					s.end());

			// process string
			if (s.empty())
			{
				if (is->eof())
				{
					is = nullptr; // become end-of-stream iterator
					return *this;
				}
				else value = T();
			}
			else
			{
				// deserialize value from string
				std::basic_istringstream<Char, CharTraits> ss(s);
				if (!detail::Deserialize(ss, value))
					is->setstate(std::ios_base::failbit);
			}

			return *this;
		}

		template <typename T, typename Char, typename CharTraits>
			greedy_istream_iterator<T, Char, CharTraits>
			greedy_istream_iterator<T, Char, CharTraits>::
			operator ++(int)
		{
			greedy_istream_iterator tmp(*this);
			++*this;
			return tmp;
		}

		/*---------------------+
		| relational operators |
		+---------------------*/

		template <typename T, typename Char, typename CharTraits>
			bool greedy_istream_iterator<T, Char, CharTraits>::operator ==(const greedy_istream_iterator<T, Char, CharTraits> &other) const
		{
			return (is && *is) == (other.is && *other.is);
		}

		template <typename T, typename Char, typename CharTraits>
			bool greedy_istream_iterator<T, Char, CharTraits>::operator !=(const greedy_istream_iterator<T, Char, CharTraits> &other) const
		{
			return !(*this == other);
		}
	}
}
