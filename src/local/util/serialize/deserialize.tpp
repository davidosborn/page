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

#include <algorithm> // copy_n, min
#include <istream>
#include <iterator> // {,back_}inserter, begin, end, istreambuf_iterator
#include <sstream> // basic_istringstream
#include <utility> // forward, move

#include "../algorithm/stdext.hpp" // copy_until
#include "../iterator/range.hpp"
#include "../iterator/repeat_iterator.hpp"
#include "../string_convert.hpp" // Convert
#include "../tuple.hpp" // tuple_{,pop_}front
#include "../type_traits/iterator.hpp" // output_iterator_value_type
#include "skip.hpp"

namespace page
{
	namespace util
	{
		namespace detail
		{
			/**
			 * Deserialize a sequence element from a stream.
			 *
			 * @return @c true if a terminating character was read.
			 */
			template <typename Char, typename CharTraits, typename T>
				bool DeserializeElement(
					std::basic_istream<Char, CharTraits> &is,
					T &element,
					const typename DeserializationDelimiter<Char, CharTraits>::Type &delimiter,
					const typename DeserializationDelimiter<Char, CharTraits>::Type &terminator = DeserializationDelimiter<Char, CharTraits>::None())
			{
				bool terminated = false;
				std::basic_string<Char, CharTraits> s;
				for (;;)
				{
					auto c = is.peek();
					if (!CharTraits::not_eof(c) || is.ignore().fail()) break;
					auto ch = CharTraits::to_char_type(c);
					if (delimiter(ch)) break;
					if (terminator(ch))
					{
						terminated = true;
						break;
					}
					s.push_back(ch);
				}
				if (!is.fail())
				{
					std::basic_istringstream<Char, CharTraits> ss(s);
					ss.copyfmt(is);
					Deserialize(ss, element);
					if (ss.fail()) is.setstate(std::ios_base::failbit);
				}
				return terminated;
			}

			/**
			 * Deserialize a sequence from a stream, writing the elements to an
			 * output iterator.
			 *
			 * @return The number of sequence elements that were deserialized.
			 */
			template <typename Char, typename CharTraits, typename OutputIterator>
				unsigned DeserializeSequence(
					std::basic_istream<Char, CharTraits> &is,
					OutputIterator result,
					typename SequenceDeserializationFlags::Type flags,
					const typename DeserializationDelimiter<Char, CharTraits>::Type &delimiter,
					const typename DeserializationDelimiter<Char, CharTraits>::Type &lastDelimiter = DeserializationDelimiter<Char, CharTraits>::None(),
					const typename DeserializationDelimiter<Char, CharTraits>::Type &terminator    = DeserializationDelimiter<Char, CharTraits>::None(),
					unsigned max = std::numeric_limits<unsigned>::max(),
					unsigned min = 0)
			{
				unsigned n = 0;

				// trim leading delimiters
				if ((flags & SequenceDeserializationFlags::trim) &&
					!Skip(is, delimiter, terminator).first) goto End;

				// check min/max limits
				if (min > max || !max) goto End;

				for (;;)
				{
					const auto &currentDelimiter(
						n != max - 1 ? delimiter : lastDelimiter);

					/*
					 * We use a temporary value, rather than passing *result
					 * directly to DeserializeElement, to support
					 * std::insert_iterator.
					 */
					typedef typename output_iterator_value_type<OutputIterator>::type T;
					T value;
					bool terminated = DeserializeElement(is, value, currentDelimiter, terminator);
					if (is.fail()) break;
					*result++ = std::move(value);
					++n;
					if (is.eof()) break;
					if (terminated)
					{
						// trim trailing terminators
						if (flags & SequenceDeserializationFlags::trim)
							Skip(is, terminator);
						break;
					}
					if (n == max)
					{
						// trim trailing delimiters
						if (flags & SequenceDeserializationFlags::trim)
							Skip(is, currentDelimiter, terminator);
						break;
					}

					// collapse consecutive delimiters
					if (flags & SequenceDeserializationFlags::collapse)
					{
						if (!Skip(is, currentDelimiter, terminator).first)
							break;
					}
					// trim trailing delimiters
					else if (flags & SequenceDeserializationFlags::trim)
					{
						auto r(Skip(is, currentDelimiter, terminator));
						if (!r.first) break;

						/*
						 * If we make it to this point, we were not at the end
						 * of the stream, so each delimiter that we trimmed was
						 * an empty element and needs to be written to the
						 * output iterator.
						 */
						auto m = std::min(r.second, max - n);
						result = std::copy_n(make_repeat_iterator(T()), m, result);
						n += m;
						if (n == max) break;
					}
				}

				End:
				if (n < min)
					is.setstate(std::ios_base::failbit);
				return n;
			}

			/**
			 * @defgroup deserialize-tuple Tuple deserialization
			 * @ingroup  deserialize
			 * @{
			 */
			template <std::size_t i, std::size_t n, std::size_t m, typename Stream, typename Tuple>
				struct DeserializeTupleImpl
			{
				typedef typename Stream::  char_type Char;
				typedef typename Stream::traits_type CharTraits;

				static Stream &Apply(
					Stream &is,
					Tuple &&t,
					typename SequenceDeserializationFlags::Type flags,
					const typename DeserializationDelimiter<Char, CharTraits>::Type &delimiter,
					const typename DeserializationDelimiter<Char, CharTraits>::Type &lastDelimiter)
				{
					DeserializeElement(is, std::get<i>(std::forward<Tuple>(t)), delimiter);
					if (is.fail()) return is;
					if (is.eof())
					{
						is.setstate(std::ios_base::failbit);
						return is;
					}

					// collapse consecutive delimiters
					if (flags & SequenceDeserializationFlags::collapse)
					{
						if (!Skip(is, delimiter).first)
						{
							is.setstate(std::ios_base::failbit);
							return is;
						}
					}

					return DeserializeTupleImpl<
						i + 1,
						n,
						m,
						Stream,
						Tuple>::Apply(
							is,
							std::forward<Tuple>(t),
							flags,
							delimiter,
							lastDelimiter);
				}
			};
			template <std::size_t n, std::size_t m, typename Stream, typename Tuple>
				struct DeserializeTupleImpl<m, n, m, Stream, Tuple>
			{
				typedef typename Stream::  char_type Char;
				typedef typename Stream::traits_type CharTraits;

				static Stream &Apply(
					Stream &is,
					Tuple &&t,
					typename SequenceDeserializationFlags::Type flags,
					const typename DeserializationDelimiter<Char, CharTraits>::Type &delimiter,
					const typename DeserializationDelimiter<Char, CharTraits>::Type &lastDelimiter)
				{
					DeserializeElement(is, std::get<m>(std::forward<Tuple>(t)), lastDelimiter);
					if (is.fail()) return is;

					return DeserializeTupleImpl<
						n,
						n,
						m,
						Stream,
						Tuple>::Apply(
							is,
							std::forward<Tuple>(t),
							flags,
							delimiter,
							lastDelimiter);
				}
			};
			template <std::size_t n, std::size_t m, typename Stream, typename Tuple>
				struct DeserializeTupleImpl<n, n, m, Stream, Tuple>
			{
				typedef typename Stream::  char_type Char;
				typedef typename Stream::traits_type CharTraits;

				static Stream &Apply(
					Stream &is,
					Tuple &&,
					typename SequenceDeserializationFlags::Type flags,
					const typename DeserializationDelimiter<Char, CharTraits>::Type &,
					const typename DeserializationDelimiter<Char, CharTraits>::Type &lastDelimiter)
				{
					// trim trailing delimiters
					if (flags & SequenceDeserializationFlags::trim)
						Skip(is, lastDelimiter);

					return is;
				}
			};
			template <typename Char, typename CharTraits, typename Tuple>
				std::basic_istream<Char, CharTraits> &DeserializeTuple(
					std::basic_istream<Char, CharTraits> &is,
					Tuple &&t,
					typename SequenceDeserializationFlags::Type flags,
					const typename DeserializationDelimiter<Char, CharTraits>::Type &delimiter,
					const typename DeserializationDelimiter<Char, CharTraits>::Type &lastDelimiter)
			{
				// trim leading delimiters
				if ((flags & SequenceDeserializationFlags::trim) &&
					!Skip(is, delimiter).first)
				{
					is.setstate(std::ios_base::failbit);
					return is;
				}

				return DeserializeTupleImpl<
					0,
					std::tuple_size<typename std::remove_reference<Tuple>::type>::value,
					std::tuple_size<typename std::remove_reference<Tuple>::type>::value - 1,
					std::basic_istream<Char, CharTraits>,
					Tuple>::Apply(
						is,
						std::forward<Tuple>(t),
						flags,
						delimiter,
						lastDelimiter);
			}
			///@}
		}

		/**
		 * @weakgroup deserialize-from-stream
		 * @{
		 */
		template <typename Char, typename CharTraits, typename T> std::basic_istream<Char, CharTraits> &Deserialize(std::basic_istream<Char, CharTraits> &is, T &value, ENABLE_IF_IMPL((is_extractable<T &, std::basic_istream<Char, CharTraits> &>::value && (!is_iterator<T>::value || std::is_pointer<T>::value) && !is_string<T>::value)))
		{
			if (!is.fail())
			{
				if (!CharTraits::not_eof(is.peek())) value = T();
				else is >> value;
			}
			return is;
		}
		template <typename Char, typename CharTraits, typename String, typename Terminator> std::basic_istream<Char, CharTraits> &Deserialize(std::basic_istream<Char, CharTraits> &is, String &s, const Terminator &terminator, ENABLE_IF_IMPL((is_string_2<String, Char, CharTraits>::value)))
		{
			if (!is.fail())
			{
				s.clear();
				copy_until(
					std::istreambuf_iterator<Char, CharTraits>(is),
					std::istreambuf_iterator<Char, CharTraits>(),
					std::back_inserter(s), terminator);
				is.ignore(); // skip terminator
			}
			return is;
		}
		template <typename Char, typename CharTraits, typename String, typename Terminator> std::basic_istream<Char, CharTraits> &Deserialize(std::basic_istream<Char, CharTraits> &is, String &s, const Terminator &terminator, ENABLE_IF_IMPL((is_string<String>::value && !is_string_2<String, Char, CharTraits>::value)))
		{
			std::basic_string<Char, CharTraits> tmp;
			if (!Deserialize(is, tmp, terminator).fail())
				try
				{
					s = std::move(Convert<
						typename String::value_type,
						typename String::traits_type>(tmp));
				}
				catch (...)
				{
					is.setstate(std::ios_base::failbit);
				}
			return is;
		}
		///@}

		/**
		 * @weakgroup deserialize-sequence-from-stream
		 * @{
		 */
		template <typename Char, typename CharTraits, typename Sequence, typename Delimiter, typename Terminator> std::basic_istream<Char, CharTraits> &Deserialize(std::basic_istream<Char, CharTraits> &is, Sequence &sequence, typename SequenceDeserializationFlags::Type flags, const Delimiter &delimiter, const Terminator &terminator, unsigned limit, ENABLE_IF_IMPL((util::is_range<Sequence>::value && !util::is_string<Sequence>::value && !util::is_extractable<Sequence &, std::basic_istream<Char, CharTraits> &>::value && util::is_compatible_with_insert_iterator<Sequence>::value)))
		{
			sequence.clear();
			return Deserialize(
				is,
				std::inserter(sequence, std::end(sequence)),
				flags,
				delimiter,
				terminator,
				limit);
		}
		template <typename Char, typename CharTraits, typename OutputIterator, typename Delimiter, typename Terminator> std::basic_istream<Char, CharTraits> &Deserialize(std::basic_istream<Char, CharTraits> &is, OutputIterator iter, typename SequenceDeserializationFlags::Type flags, const Delimiter &delimiterArg, const Terminator &terminator, unsigned limit, ENABLE_IF_IMPL(is_iterator<OutputIterator>::value && !std::is_pointer<OutputIterator>::value))
		{
			auto delimiter(DeserializationDelimiter<Char, CharTraits>::Normalize(delimiterArg));
			detail::DeserializeSequence(
				is,
				iter,
				flags,
				delimiter,
				delimiter,
				DeserializationDelimiter<Char, CharTraits>::Normalize(terminator),
				limit);
			return is;
		}
		template <typename Char, typename CharTraits, typename Range, typename Delimiter, typename LastDelimiter> std::basic_istream<Char, CharTraits> &Deserialize(std::basic_istream<Char, CharTraits> &is, const Range &range, typename SequenceDeserializationFlags::Type flags, const Delimiter &delimiter, const LastDelimiter &lastDelimiter, ENABLE_IF_IMPL((util::is_range<Range>::value && !util::is_const_propogated<Range>::value && !util::is_string<Range>::value && !util::is_extractable<Range &, std::basic_istream<Char, CharTraits> &>::value && !util::is_compatible_with_insert_iterator<Range>::value)))
		{
			auto n = std::distance(std::begin(range), std::end(range));
			detail::DeserializeSequence(
				is,
				std::begin(range),
				flags,
				DeserializationDelimiter<Char, CharTraits>::Normalize(delimiter),
				DeserializationDelimiter<Char, CharTraits>::Normalize(lastDelimiter),
				DeserializationDelimiter<Char, CharTraits>::None(),
				n,
				n);
			return is;
		}
		template <typename Char, typename CharTraits, typename Range, typename Delimiter, typename LastDelimiter> std::basic_istream<Char, CharTraits> &Deserialize(std::basic_istream<Char, CharTraits> &is, Range &range, typename SequenceDeserializationFlags::Type flags, const Delimiter &delimiter, const LastDelimiter &lastDelimiter, ENABLE_IF_IMPL((util::is_range<Range>::value && util::is_const_propogated<Range>::value && !std::is_const<Range>::value && !util::is_string<Range>::value && !util::is_extractable<Range &, std::basic_istream<Char, CharTraits> &>::value && !util::is_compatible_with_insert_iterator<Range>::value)))
		{
			auto n = std::distance(std::begin(range), std::end(range));
			detail::DeserializeSequence(
				is,
				std::begin(range),
				flags,
				DeserializationDelimiter<Char, CharTraits>::Normalize(delimiter),
				DeserializationDelimiter<Char, CharTraits>::Normalize(lastDelimiter),
				DeserializationDelimiter<Char, CharTraits>::None(),
				n,
				n);
			return is;
		}
		template <typename Char, typename CharTraits, typename Iterator, typename Delimiter, typename LastDelimiter> std::basic_istream<Char, CharTraits> &Deserialize(std::basic_istream<Char, CharTraits> &is, Iterator first, Iterator last, typename SequenceDeserializationFlags::Type flags, const Delimiter &delimiter, const LastDelimiter &lastDelimiter, ENABLE_IF_IMPL(is_iterator<Iterator>::value))
		{
			return Deserialize(
				is,
				make_range(first, last),
				flags,
				delimiter,
				lastDelimiter);
		}
		template <typename Char, typename CharTraits, typename... Types, typename Delimiter, typename LastDelimiter> std::basic_istream<Char, CharTraits> &Deserialize(std::basic_istream<Char, CharTraits> &is, std::tuple<Types...> &t, typename SequenceDeserializationFlags::Type flags, const Delimiter &delimiter, const LastDelimiter &lastDelimiter)
		{
			return detail::DeserializeTuple(is, t, flags,
				DeserializationDelimiter<Char, CharTraits>::Normalize(delimiter),
				DeserializationDelimiter<Char, CharTraits>::Normalize(lastDelimiter));
		}
		template <typename Char, typename CharTraits, typename... Types, typename Delimiter, typename LastDelimiter> std::basic_istream<Char, CharTraits> &Deserialize(std::basic_istream<Char, CharTraits> &is, std::tuple<Types...> &&t, typename SequenceDeserializationFlags::Type flags, const Delimiter &delimiter, const LastDelimiter &lastDelimiter)
		{
			return detail::DeserializeTuple(is, t, flags,
				DeserializationDelimiter<Char, CharTraits>::Normalize(delimiter),
				DeserializationDelimiter<Char, CharTraits>::Normalize(lastDelimiter));
		}
		template <typename Char, typename CharTraits, typename First, typename Second, typename Delimiter, typename LastDelimiter> std::basic_istream<Char, CharTraits> &Deserialize(std::basic_istream<Char, CharTraits> &is, std::pair<First, Second> &p, typename SequenceDeserializationFlags::Type flags, const Delimiter &delimiter, const LastDelimiter &lastDelimiter)
		{
			return Deserialize(is, std::tuple<First, Second>(p), flags, delimiter, lastDelimiter);
		}
		template <typename Char, typename CharTraits, typename First, typename Second, typename Delimiter, typename LastDelimiter> std::basic_istream<Char, CharTraits> &Deserialize(std::basic_istream<Char, CharTraits> &is, std::pair<First, Second> &&p, typename SequenceDeserializationFlags::Type flags, const Delimiter &delimiter, const LastDelimiter &lastDelimiter)
		{
			return Deserialize(is, std::tuple<First, Second>(p), flags, delimiter, lastDelimiter);
		}
		///@}

		/**
		 * @weakgroup deserialize-from-stream
		 * @{
		 */
		template <typename Char, typename CharTraits, typename... Args> std::basic_istream<Char, CharTraits> &Deserialize(std::basic_istream<Char, CharTraits> &&os, Args &&... args)
		{
			return Deserialize(os, std::forward<Args>(args)...);
		}
		///@}
	}
}
