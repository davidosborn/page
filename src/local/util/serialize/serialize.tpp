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

#include <iterator> // begin, end
#include <ostream>
#include <utility> // forward, move
#include "../iterator/range.hpp"
#include "../string_convert.hpp" // Convert
#include "../tuple.hpp" // tuple_{,pop_}front

namespace page
{
	namespace util
	{
		namespace detail
		{
			/**
			 * @defgroup serialize-tuple Tuple serialization
			 * @ingroup  serialize
			 *
			 * @{
			 */
			template <std::size_t i, std::size_t n, std::size_t m, typename Stream, typename Tuple>
				struct SerializeTupleImpl
			{
				typedef typename Stream::  char_type Char;
				typedef typename Stream::traits_type CharTraits;

				static Stream &Apply(
					Stream &os,
					const Tuple &t,
					const typename SerializationDelimiter<Char, CharTraits>::Type &delimiter)
				{
					Serialize(os, std::get<i>(t));
					Serialize(os, delimiter);

					return SerializeTupleImpl<
						i + 1,
						n,
						m,
						Stream,
						Tuple>::Apply(
							os,
							t,
							delimiter);
				}
			};
			template <std::size_t n, std::size_t m, typename Stream, typename Tuple>
				struct SerializeTupleImpl<m, n, m, Stream, Tuple>
			{
				typedef typename Stream::  char_type Char;
				typedef typename Stream::traits_type CharTraits;

				static Stream &Apply(
					Stream &os,
					const Tuple &t,
					const typename SerializationDelimiter<Char, CharTraits>::Type &delimiter)
				{
					return Serialize(os, std::get<m>(t));
				}
			};
			template <std::size_t n, std::size_t m, typename Stream, typename Tuple>
				struct SerializeTupleImpl<n, n, m, Stream, Tuple>
			{
				typedef typename Stream::  char_type Char;
				typedef typename Stream::traits_type CharTraits;

				static Stream &Apply(
					Stream &os,
					const Tuple &,
					const typename SerializationDelimiter<Char, CharTraits>::Type &)
				{
					return os;
				}
			};
			template <typename Char, typename CharTraits, typename Tuple>
				std::basic_ostream<Char, CharTraits> &SerializeTuple(
					std::basic_ostream<Char, CharTraits> &os,
					const Tuple &t,
					const typename SerializationDelimiter<Char, CharTraits>::Type &delimiter)
			{
				return SerializeTupleImpl<
					0,
					std::tuple_size<Tuple>::value,
					std::tuple_size<Tuple>::value - 1,
					std::basic_ostream<Char, CharTraits>,
					Tuple>::Apply(
						os,
						t,
						delimiter);
			}
			///@}
		}

		/**
		 * @weakgroup serialize-to-stream
		 * @{
		 */
		template <typename Char, typename CharTraits, typename T> std::basic_ostream<Char, CharTraits> &Serialize(std::basic_ostream<Char, CharTraits> &os, const T &value, ENABLE_IF_IMPL((is_insertable<const T &, std::basic_ostream<Char, CharTraits> &>::value)))
		{
			return os << value;
		}
		template <typename Char, typename CharTraits, typename String> std::basic_ostream<Char, CharTraits> &Serialize(std::basic_ostream<Char, CharTraits> &os, const String &s, ENABLE_IF_IMPL((is_string<String>::value && !is_string_2<String, Char, CharTraits>::value)))
		{
			std::basic_string<Char, CharTraits> tmp;
			try
			{
				tmp = std::move(Convert<Char, CharTraits>(s));
			}
			catch (...)
			{
					os.setstate(std::ios_base::failbit);
			}
			return Serialize(os, tmp);
		}
		///@}

		/**
		 * @weakgroup serialize-sequence-to-stream
		 * @{
		 */
		template <typename Char, typename CharTraits, typename Range, typename Delimiter> std::basic_ostream<Char, CharTraits> &Serialize(std::basic_ostream<Char, CharTraits> &os, const Range &range, const Delimiter &delimiterArg, ENABLE_IF_IMPL((is_range<Range>::value && !is_string<Range>::value && !is_insertable<const Range &, std::basic_ostream<Char, CharTraits> &>::value)))
		{
			auto delimiter(SerializationDelimiter<Char, CharTraits>::Normalize(delimiterArg));

			// check for empty sequence
			if (std::begin(range) == std::end(range))
				return os;

			for (auto iter(std::begin(range));;)
			{
				Serialize(os, *iter);

				if (++iter == std::end(range))
					break;

				Serialize(os, delimiter);
			}
			return os;
		}
		template <typename Char, typename CharTraits, typename Iterator, typename Delimiter> std::basic_ostream<Char, CharTraits> &Serialize(std::basic_ostream<Char, CharTraits> &os, Iterator first, Iterator last, const Delimiter &delimiter, ENABLE_IF_IMPL(is_iterator<Iterator>::value))
		{
			return Serialize(os, make_range(first, last), delimiter);
		}
		template <typename Char, typename CharTraits, typename... Types, typename Delimiter> std::basic_ostream<Char, CharTraits> &Serialize(std::basic_ostream<Char, CharTraits> &os, const std::tuple<Types...> &t, const Delimiter &delimiter)
		{
			return detail::SerializeTuple(os, t,
				SerializationDelimiter<Char, CharTraits>::Normalize(delimiter));
		}
		template <typename Char, typename CharTraits, typename First, typename Second, typename Delimiter> std::basic_ostream<Char, CharTraits> &Serialize(std::basic_ostream<Char, CharTraits> &os, const std::pair<First, Second> &p, const Delimiter &delimiter)
		{
			return Serialize(os, std::tuple<First, Second>(p), delimiter);
		}
		///@}

		/**
		 * @weakgroup serialize-to-stream
		 * @{
		 */
		template <typename Char, typename CharTraits, typename... Args> std::basic_ostream<Char, CharTraits> &Serialize(std::basic_ostream<Char, CharTraits> &&os, Args &&... args)
		{
			return Serialize(os, std::forward<Args>(args)...);
		}
		///@}
	}
}
