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

#include <algorithm> // equal, find{,_if{,_not}}
#include <iterator> // back_inserter, istreambuf_iterator
#include <tuple>
#include "algorithm/stdext.hpp" // copy_until
#include "serialize/deserialize_string.hpp" // Deserialize
#include "serialize/serialize_string.hpp" // Serialize

namespace page
{
	namespace util
	{
		// Join
		template <typename Char, typename CharTraits, typename... Parts> std::basic_string<Char, CharTraits> Join(Parts &&... parts)
		{
			Serialize<Char, CharTraits>(std::tuple<Parts &&...>(std::forward<Parts>(parts)...), "");
		}

		// Split
		template <typename Char, typename CharTraits, typename OutputIterator, typename Delimiter> void Split(const std::basic_string<Char, CharTraits> &s, OutputIterator result, typename SequenceDeserializationFlags::Type flags, const Delimiter &delimiter, unsigned limit)
		{
			Deserialize(s, result, flags, delimiter, limit);
		}
		template <typename Char, typename OutputIterator, typename Delimiter> void Split(const std::basic_string<Char> &s, OutputIterator result, typename SequenceDeserializationFlags::Type flags, const Delimiter &delimiter, unsigned limit)
		{
			return Split(std::basic_string<Char>(s), result, flags, delimiter, limit);
		}

		// Partition
		template <typename Char, typename CharTraits, typename Delimiter> std::pair<std::basic_string<Char, CharTraits>, std::basic_string<Char, CharTraits>> Partition(const std::basic_string<Char, CharTraits> &s, typename SequenceDeserializationFlags::Type flags, const Delimiter &delimiter)
		{
			std::pair<
				std::basic_string<Char, CharTraits>,
				std::basic_string<Char, CharTraits>> r;
			Deserialize(s, r, flags, delimiter);
			return r;
		}
		template <typename Char, typename Delimiter> std::pair<std::basic_string<Char>, std::basic_string<Char>> Partition(const char *s, typename SequenceDeserializationFlags::Type flags, const Delimiter &delimiter)
		{
			return Partition(std::basic_string<Char>(s), flags, delimiter);
		}

		// Trim
		template <typename Char, typename CharTraits, typename Delimiter> std::basic_string<Char, CharTraits> Trim(const std::basic_string<Char, CharTraits> &s, const Delimiter &delimiter)
		{
			return TrimLeading(TrimTrailing(s, delimiter), delimiter);
		}
		template <typename Char, typename CharTraits, typename Delimiter> std::basic_string<Char, CharTraits> TrimLeading(const std::basic_string<Char, CharTraits> &s, const Delimiter &delimiter)
		{
			return std::basic_string<Char, CharTraits>(
				std::find_if_not(s.begin(), s.end(),
					DeserializationDelimiter<Char, CharTraits>::Normalize(delimiter)),
				s.end());
		}
		template <typename Char, typename CharTraits, typename Delimiter> std::basic_string<Char, CharTraits> TrimTrailing(const std::basic_string<Char, CharTraits> &s, const Delimiter &delimiter)
		{
			return std::basic_string<Char, CharTraits>(
				s.begin(),
				std::find_if_not(s.rbegin(), s.rend(),
					DeserializationDelimiter<Char, CharTraits>::Normalize(delimiter)).base());
		}

		// NormalizeNewlines
		template <typename Char, typename CharTraits> std::basic_string<Char, CharTraits> NormalizeNewlines(const std::basic_string<Char, CharTraits> &s)
		{
			std::basic_string<Char, CharTraits> r;
			r.reserve(s.size());
			Char newline = '\0';
			for (auto c : s)
			{
				switch (c)
				{
					case '\n':
					case '\r':
					if (newline == '\0' || newline == c)
					{
						r.push_back('\n');
						newline = c;
					}
					else newline = '\0';
					break;
					
					default: r.push_back(c);
				}
			}
			return r;
		}

		// getline
		template <typename Char, typename CharTraits> std::basic_istream<Char, CharTraits> &getline(std::basic_istream<Char, CharTraits> &is, std::basic_string<Char, CharTraits> &s)
		{
			if (!is) return is;
			s.clear();
			copy_until(
				std::istreambuf_iterator<Char, CharTraits>(is),
				std::istreambuf_iterator<Char, CharTraits>(),
				std::back_inserter(s), isnewline_function<Char>());
			if (is.good())
			{
				Char
					a = CharTraits::to_char_type(is.get()),
					b = CharTraits::to_char_type(is.peek());
				switch (b)
				{
					case '\n':
					case '\r': if (a != b) is.ignore();
				}
			}
			return is;
		}
		template <typename Char, typename CharTraits, typename Delimiter> std::basic_istream<Char, CharTraits> &getline(std::basic_istream<Char, CharTraits> &is, std::basic_string<Char, CharTraits> &s, const Delimiter &delimiter)
		{
			return Deserialize(is, s, delimiter);
		}

		// StartsWith/EndsWith
		template <typename Char, typename CharTraits> bool StartsWith(const std::basic_string<Char, CharTraits> &a, const std::basic_string<Char, CharTraits> &b)
		{
			return a.size() >= b.size() &&
				std::equal(b.begin(), b.end(), a.begin());
		}
		template <typename Char, typename CharTraits> bool EndsWith(const std::basic_string<Char, CharTraits> &a, const std::basic_string<Char, CharTraits> &b)
		{
			return a.size() >= b.size() &&
				std::equal(b.begin(), b.end(), a.end() - b.size());
		}
	}
}
