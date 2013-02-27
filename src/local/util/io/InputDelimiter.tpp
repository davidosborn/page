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

#include <algorithm> // find
#include <cstring> // strchr
#include <iterator> // end, begin
#include <locale> // isspace

namespace page
{
	namespace util
	{
		/*--------------------------+
		| constructors & destructor |
		+--------------------------*/

		template <typename Char, typename CharTraits>
			InputDelimiter<Char, CharTraits>::InputDelimiter() {}

		template <typename Char, typename CharTraits>
			InputDelimiter<Char, CharTraits>::InputDelimiter(Char c) :
				InputDelimiter([c](Char x) { return CharTraits::eq(x, c); }) {}

		template <typename Char, typename CharTraits>
			InputDelimiter<Char, CharTraits>::InputDelimiter(const Char *s) :
				InputDelimiter([s](Char c) { return std::strchr(s, c); }) {}

		template <typename Char, typename CharTraits> template <typename InputRange>
			InputDelimiter<Char, CharTraits>::InputDelimiter(InputRange range, ENABLE_IF_IMPL((is_range<InputRange>::value))) :
				InputDelimiter([&range](Char c) { return std::find(std::begin(range), std::end(range), c) != std::end(range); }) {}

		template <typename Char, typename CharTraits> template <typename InputIterator>
			InputDelimiter<Char, CharTraits>::InputDelimiter(InputIterator first, InputIterator last, ENABLE_IF_IMPL((is_iterator<InputIterator>::value))) :
				InputDelimiter([&first, &last](Char c) { return std::find(first, last, c) != last; }) {}

		template <typename Char, typename CharTraits>
			InputDelimiter<Char, CharTraits>::InputDelimiter(const Predicate &predicate) :
				predicate(predicate) {}

		/*----------+
		| observers |
		+----------*/

		template <typename Char, typename CharTraits>
			const typename InputDelimiter<Char, CharTraits>::Predicate &
			InputDelimiter<Char, CharTraits>::GetPredicate() const
		{
			return predicate;
		}

		template <typename Char, typename CharTraits>
			bool InputDelimiter<Char, CharTraits>::operator()(Char c) const
		{
			return predicate ? predicate(c) : false;
		}

		template <typename Char, typename CharTraits>
			InputDelimiter<Char, CharTraits>::operator bool() const
		{
			return predicate != nullptr;
		}

		/*----------------------+
		| predefined delimiters |
		+----------------------*/

		template <typename Char, typename CharTraits>
			const InputDelimiter<Char, CharTraits> &
			InputDelimiter<Char, CharTraits>::GetEmptyDelimiter()
		{
			static InputDelimiter r;
			return r;
		}

		template <typename Char, typename CharTraits>
			const InputDelimiter<Char, CharTraits> &
			InputDelimiter<Char, CharTraits>::GetSpaceDelimiter()
		{
			static InputDelimiter r([](Char c) { return std::isspace(c, std::locale()); });
			return r;
		}
	}
}