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

#include <iterator> // begin, end
#include <ostream>
#include <utility> // forward, move

#include "../../err/Exception.hpp"
#include "../iterator/range.hpp"
#include "../string/convert.hpp" // Convert
#include "../tuple.hpp" // tuple_{,pop_}front

namespace page
{
	namespace util
	{
		/**
		 * @weakgroup serialize
		 * @{
		 */
		/**
		 * @weakgroup serialize-stream
		 * @{
		 */
		/**
		 * @weakgroup serialize-stream-value
		 * @{
		 */
		template <typename Char, typename CharTraits, typename T>
			std::basic_ostream<Char, CharTraits> &Serialize(
				std::basic_ostream<Char, CharTraits> &os,
				const T &value,
				ENABLE_IF_IMPL((is_insertable<const T &, std::basic_ostream<Char, CharTraits> &>::value)))
		{
			return os << value;
		}

		/**
		 * @weakgroup serialize-stream-value-string
		 * @{
		 */
		template <typename Char, typename CharTraits, typename String>
			std::basic_ostream<Char, CharTraits> &Serialize(
				std::basic_ostream<Char, CharTraits> &os,
				const String &s,
				ENABLE_IF_IMPL((
					is_string<String>::value &&
					!is_string_2<String, Char, CharTraits>::value)))
		{
			std::basic_string<Char, CharTraits> tmp;
			try
			{
				tmp = std::move(Convert<Char, CharTraits>(s));
			}
			catch (const std::exception &)
			{
				os.setstate(std::ios_base::failbit);
			}
			return Serialize(os, tmp);
		}
		///@}
		///@}

		/**
		 * @weakgroup serialize-stream-sequence
		 * @{
		 */
		/**
		 * @weakgroup serialize-stream-sequence-homogeneous
		 * @{
		 */
		template <typename Char, typename CharTraits, typename InputRange, typename Separator>
			std::basic_ostream<Char, CharTraits> &Serialize(
				std::basic_ostream<Char, CharTraits> &os,
				InputRange range,
				Separator separatorArg,
				ENABLE_IF_IMPL((
					is_range<InputRange>::value &&
					!is_string<InputRange>::value &&
					!is_insertable<InputRange, std::basic_ostream<Char, CharTraits> &>::value)))
		{
			OutputDelimiter<Char, CharTraits> separator(separatorArg);

			// check for empty sequence
			if (std::begin(range) == std::end(range))
				return os;

			for (auto iter(std::begin(range));;)
			{
				Serialize(os, *iter);

				if (++iter == std::end(range))
					break;

				Serialize(os, separator);
			}
			return os;
		}

		template <typename Char, typename CharTraits, typename InputIterator, typename Separator>
			std::basic_ostream<Char, CharTraits> &Serialize(
				std::basic_ostream<Char, CharTraits> &os,
				InputIterator first,
				InputIterator last,
				Separator separator,
				ENABLE_IF_IMPL((is_iterator<InputIterator>::value)))
		{
			return Serialize(os, make_range(first, last),
				OutputDelimiter<Char, CharTraits>(separator));
		}
		///@}

		/**
		 * @weakgroup serialize-stream-sequence-heterogeneous
		 * @{
		 */
		namespace detail
		{
			template <std::size_t i, std::size_t n, std::size_t m, typename Stream, typename Tuple>
				struct SerializeTupleImpl
			{
				typedef typename Stream::  char_type Char;
				typedef typename Stream::traits_type CharTraits;

				static Stream &Apply(
					Stream &os,
					const Tuple &t,
					const OutputDelimiter<Char, CharTraits> &separator)
				{
					Serialize(os, std::get<i>(t));
					Serialize(os, separator);

					return SerializeTupleImpl<
						i + 1,
						n,
						m,
						Stream,
						Tuple>::Apply(
							os,
							t,
							separator);
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
					const OutputDelimiter<Char, CharTraits> &separator)
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
					const OutputDelimiter<Char, CharTraits> &)
				{
					return os;
				}
			};

			template <typename Char, typename CharTraits, typename Tuple>
				std::basic_ostream<Char, CharTraits> &SerializeTuple(
					std::basic_ostream<Char, CharTraits> &os,
					const Tuple &t,
					const OutputDelimiter<Char, CharTraits> &separator)
			{
				return SerializeTupleImpl<
					0,
					std::tuple_size<Tuple>::value,
					std::tuple_size<Tuple>::value - 1,
					std::basic_ostream<Char, CharTraits>,
					Tuple>::Apply(
						os,
						t,
						separator);
			}
		}

		template <typename Char, typename CharTraits, typename... Types, typename Separator>
			std::basic_ostream<Char, CharTraits> &Serialize(
				std::basic_ostream<Char, CharTraits> &os,
				const std::tuple<Types...> &t,
				Separator separator)
		{
			return detail::SerializeTuple(os, t,
				OutputDelimiter<Char, CharTraits>(separator));
		}

		template <typename Char, typename CharTraits, typename First, typename Second, typename Separator>
			std::basic_ostream<Char, CharTraits> &Serialize(
				std::basic_ostream<Char, CharTraits> &os,
				const std::pair<First, Second> &p,
				Separator separator)
		{
			return Serialize(os, std::tuple<First, Second>(p),
				OutputDelimiter<Char, CharTraits>(separator));
		}
		///@}
		///@}

		template <typename Char, typename CharTraits, typename... Args>
			void Serialize(std::basic_ostream<Char, CharTraits> &&os, Args &&... args)
		{
			if (Serialize(os, std::forward<Args>(args)...).fail())
				THROW((err::Exception<err::UtilModuleTag, err::SerializationTag>()))
		}
		///@}

		/**
		 * @weakgroup deserialize-string
		 * @{
		 */
		template <typename Char, typename CharTraits, typename Allocator, typename Arg, typename... Args>
			auto Serialize(Arg &&arg, Args &&... args) ->
				ENABLE_IF_RETURN(
					!(std::is_base_of<std::ios_base, typename std::remove_reference<Arg>::type>::value),
					std::basic_string<Char COMMA CharTraits COMMA Allocator>)
		{
			std::basic_ostringstream<Char, CharTraits, Allocator> ss;
			if (Serialize(ss, std::forward<Arg>(arg), std::forward<Args>(args)...).fail())
				THROW((err::Exception<err::UtilModuleTag, err::SerializationTag>()))
			return ss.str();
		}
		///@}
		///@}
	}
}
