#include <algorithm> // fill_n, min
#include <istream>
#include <iterator> // {,back_}inserter, begin, end, istreambuf_iterator
#include <sstream> // basic_istringstream
#include <utility> // forward, move

#include <boost/range/iterator_range.hpp>

#include "../../err/Exception.hpp"
#include "../algorithm/stdext.hpp" // copy_until
#include "../container/tuple.hpp" // tuple_{,pop_}front
#include "../locale/convert.hpp" // Convert
#include "../type_traits/iterator.hpp" // output_iterator_value_type
#include "skip.hpp" // Skip

namespace page
{
	namespace util
	{
		/**
		 * @weakgroup deserialize
		 * @{
		 */
		/**
		 * @weakgroup deserialize-stream
		 * @{
		 */
		/**
		 * @weakgroup deserialize-stream-value
		 * @{
		 */
		template <typename Char, typename CharTraits, typename T>
			std::basic_istream<Char, CharTraits> &Deserialize(
				std::basic_istream<Char, CharTraits> &is,
				T &value,
				ENABLE_IF_IMPL((
					is_extractable<T &, std::basic_istream<Char, CharTraits> &>::value &&
					(!is_iterator<T>::value || std::is_pointer<T>::value) &&
					!is_string<T>::value)))
		{
			if (!is.fail())
			{
				if (!CharTraits::not_eof(is.peek())) value = T();
				else is >> value;
			}
			return is;
		}

		/**
		 * @weakgroup deserialize-stream-value-string
		 * @{
		 */
		template <typename Char, typename CharTraits, typename String, typename Terminator>
			std::basic_istream<Char, CharTraits> &Deserialize(
				std::basic_istream<Char, CharTraits> &is,
				String &s,
				Terminator terminator,
				ENABLE_IF_IMPL((is_string_2<String, Char, CharTraits>::value)))
		{
			if (!is.fail())
			{
				s.clear();
				copy_until(
					std::istreambuf_iterator<Char, CharTraits>(is),
					std::istreambuf_iterator<Char, CharTraits>(),
					std::back_inserter(s),
					InputDelimiter<Char, CharTraits>(terminator));
				is.ignore(); // skip terminator
			}
			return is;
		}

		template <
			CharEncoding ToCharEncoding, CharEncoding FromCharEncoding,
			typename Char, typename CharTraits, typename String, typename Terminator>
			std::basic_istream<Char, CharTraits> &Deserialize(
				std::basic_istream<Char, CharTraits> &is,
				String &s,
				Terminator terminator,
				ENABLE_IF_IMPL((
					is_string<String>::value &&
					!is_string_2<String, Char, CharTraits>::value)))
		{
			std::basic_string<Char, CharTraits> tmp;
			if (!Deserialize(is, tmp, terminator).fail())
				try
				{
					s = std::move(Convert<
						ToCharEncoding, FromCharEncoding,
						typename String::value_type,
						typename String::traits_type,
						typename String::allocator_type>(tmp));
				}
				catch (const std::exception &)
				{
					is.setstate(std::ios_base::failbit);
				}
			return is;
		}
		///@}
		///@}

		/**
		 * @weakgroup deserialize-stream-sequence
		 * @{
		 */
		/**
		 * @weakgroup deserialize-stream-sequence-homogeneous
		 * @{
		 */
		namespace detail
		{
			/**
			 * Deserializes a sequence element from a stream.
			 *
			 * @return @c true if a terminating character was read.
			 */
			template <typename Char, typename CharTraits, typename T>
				bool DeserializeElement(
					std::basic_istream<Char, CharTraits> &is,
					T &element,
					const InputDelimiter<Char, CharTraits> &separator,
					const InputDelimiter<Char, CharTraits> &terminator = InputDelimiter<Char, CharTraits>())
			{
				bool terminated = false;
				std::basic_string<Char, CharTraits> s;
				for (;;)
				{
					auto c = is.peek();
					if (!CharTraits::not_eof(c) || is.ignore().fail()) break;
					auto ch = CharTraits::to_char_type(c);
					if (separator(ch)) break;
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
			 * Deserializes a sequence from a stream, writing the elements to an
			 * output iterator.
			 *
			 * @return The number of sequence elements that were deserialized.
			 */
			template <typename Char, typename CharTraits, typename OutputIterator>
				unsigned DeserializeSequence(
					std::basic_istream<Char, CharTraits> &is,
					OutputIterator result,
					typename SequenceDeserializationFlags::Type flags,
					const InputDelimiter<Char, CharTraits> &separator,
					const InputDelimiter<Char, CharTraits> &lastSeparator = InputDelimiter<Char, CharTraits>(),
					const InputDelimiter<Char, CharTraits> &terminator    = InputDelimiter<Char, CharTraits>(),
					unsigned max = std::numeric_limits<unsigned>::max(),
					unsigned min = 0)
			{
				unsigned n = 0;

				// trim leading separators
				if ((flags & SequenceDeserializationFlags::trim) &&
					!Skip(is, separator, terminator).first) goto End;

				// check min/max limits
				if (min > max || !max) goto End;

				for (;;)
				{
					const auto &currentSeparator(
						n != max - 1 ? separator : lastSeparator);

					/*
					 * We use a temporary value, rather than passing *result
					 * directly to DeserializeElement, to support
					 * std::insert_iterator.
					 */
					typedef typename output_iterator_value_type<OutputIterator>::type T;
					T value;
					bool terminated = DeserializeElement(is, value, currentSeparator, terminator);
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
						// trim trailing separators
						if (flags & SequenceDeserializationFlags::trim)
							Skip(is, currentSeparator, terminator);
						break;
					}

					// collapse consecutive separators
					if (flags & SequenceDeserializationFlags::collapse)
					{
						if (!Skip(is, currentSeparator, terminator).first)
							break;
					}
					// trim trailing separators
					else if (flags & SequenceDeserializationFlags::trim)
					{
						auto r(Skip(is, currentSeparator, terminator));
						if (!r.first) break;

						/*
						 * If we make it to this point, we were not at the end
						 * of the stream, so each separator that we trimmed was
						 * an empty element and needs to be written to the
						 * output iterator.
						 */
						auto m = std::min(r.second, max - n);
						result = std::fill_n(result, m, T());
						n += m;
						if (n == max) break;
					}
				}

				End:
				if (n < min)
					is.setstate(std::ios_base::failbit);
				return n;
			}
		}

		/**
		 * @weakgroup deserialize-stream-sequence-homogeneous-unlimited
		 * @{
		 */
		template <typename Char, typename CharTraits, typename Container, typename Separator, typename Terminator>
			std::basic_istream<Char, CharTraits> &Deserialize(
				std::basic_istream<Char, CharTraits> &is,
				Container &container,
				typename SequenceDeserializationFlags::Type flags,
				Separator separator,
				Terminator terminator,
				unsigned limit,
				ENABLE_IF_IMPL((
					util::is_range<Container>::value &&
					!util::is_string<Container>::value &&
					!util::is_extractable<Container &, std::basic_istream<Char, CharTraits> &>::value &&
					util::is_compatible_with_insert_iterator<Container>::value)))
		{
			container.clear();
			return Deserialize(
				is,
				std::inserter(container, std::end(container)),
				flags,
				InputDelimiter<Char, CharTraits>(separator),
				InputDelimiter<Char, CharTraits>(terminator),
				limit);
		}

		template <typename Char, typename CharTraits, typename OutputIterator, typename Separator, typename Terminator>
			std::basic_istream<Char, CharTraits> &Deserialize(
				std::basic_istream<Char, CharTraits> &is,
				OutputIterator iter,
				typename SequenceDeserializationFlags::Type flags,
				Separator separatorArg,
				Terminator terminator,
				unsigned limit,
				ENABLE_IF_IMPL((
					is_iterator<OutputIterator>::value &&
					!std::is_pointer<OutputIterator>::value)))
		{
			InputDelimiter<Char, CharTraits> separator(separatorArg);
			detail::DeserializeSequence(
				is,
				iter,
				flags,
				separator,
				separator,
				InputDelimiter<Char, CharTraits>(terminator),
				limit);
			return is;
		}
		///@}

		/**
		 * @weakgroup deserialize-stream-sequence-homogeneous-limited
		 * @{
		 */
		template <typename Char, typename CharTraits, typename Container, typename Separator, typename LastSeparator>
			std::basic_istream<Char, CharTraits> &Deserialize(
				std::basic_istream<Char, CharTraits> &is,
				const Container &container,
				typename SequenceDeserializationFlags::Type flags,
				Separator separator,
				LastSeparator lastSeparator,
				ENABLE_IF_IMPL((
					util::is_range<Container>::value &&
					!util::is_const_propogated<Container>::value &&
					!util::is_string<Container>::value &&
					!util::is_extractable<Container &, std::basic_istream<Char, CharTraits> &>::value &&
					!util::is_compatible_with_insert_iterator<Container>::value)))
		{
			auto n = std::distance(std::begin(container), std::end(container));
			detail::DeserializeSequence(
				is,
				std::begin(container),
				flags,
				InputDelimiter<Char, CharTraits>(separator),
				InputDelimiter<Char, CharTraits>(lastSeparator),
				InputDelimiter<Char, CharTraits>(),
				n,
				n);
			return is;
		}

		template <typename Char, typename CharTraits, typename Container, typename Separator, typename LastSeparator>
			std::basic_istream<Char, CharTraits> &Deserialize(
				std::basic_istream<Char, CharTraits> &is,
				Container &container,
				typename SequenceDeserializationFlags::Type flags,
				Separator separator,
				LastSeparator lastSeparator,
				ENABLE_IF_IMPL((
					util::is_range<Container>::value &&
					!util::is_string<Container>::value &&
					!util::is_extractable<Container &, std::basic_istream<Char, CharTraits> &>::value &&
					!util::is_compatible_with_insert_iterator<Container>::value &&
					util::is_const_propogated<Container>::value && !std::is_const<Container>::value)))
		{
			auto n = std::distance(std::begin(container), std::end(container));
			detail::DeserializeSequence(
				is,
				std::begin(container),
				flags,
				InputDelimiter<Char, CharTraits>(separator),
				InputDelimiter<Char, CharTraits>(lastSeparator),
				InputDelimiter<Char, CharTraits>(),
				n,
				n);
			return is;
		}

		template <typename Char, typename CharTraits, typename InputIterator, typename Separator, typename LastSeparator>
			std::basic_istream<Char, CharTraits> &Deserialize(
				std::basic_istream<Char, CharTraits> &is,
				InputIterator first,
				InputIterator last,
				typename SequenceDeserializationFlags::Type flags,
				Separator separator,
				LastSeparator lastSeparator,
				ENABLE_IF_IMPL((is_iterator<InputIterator>::value)))
		{
			return Deserialize(
				is,
				boost::make_iterator_range(first, last),
				flags,
				InputDelimiter<Char, CharTraits>(separator),
				InputDelimiter<Char, CharTraits>(lastSeparator));
		}
		///@}
		///@}

		/**
		 * @weakgroup deserialize-stream-sequence-heterogeneous
		 * @{
		 */
		namespace detail
		{
			template <std::size_t i, std::size_t n, std::size_t m, typename Stream, typename Tuple>
				struct DeserializeTupleImpl
			{
				typedef typename Stream::char_type   Char;
				typedef typename Stream::traits_type CharTraits;

				static Stream &Apply(
					Stream &is,
					Tuple &&t,
					typename SequenceDeserializationFlags::Type flags,
					const InputDelimiter<Char, CharTraits> &separator,
					const InputDelimiter<Char, CharTraits> &lastSeparator)
				{
					DeserializeElement(is, std::get<i>(std::forward<Tuple>(t)), separator);
					if (is.fail()) return is;
					if (is.eof())
					{
						is.setstate(std::ios_base::failbit);
						return is;
					}

					// collapse consecutive separators
					if (flags & SequenceDeserializationFlags::collapse)
					{
						if (!Skip(is, separator).first)
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
							separator,
							lastSeparator);
				}
			};

			template <std::size_t n, std::size_t m, typename Stream, typename Tuple>
				struct DeserializeTupleImpl<m, n, m, Stream, Tuple>
			{
				typedef typename Stream::char_type   Char;
				typedef typename Stream::traits_type CharTraits;

				static Stream &Apply(
					Stream &is,
					Tuple &&t,
					typename SequenceDeserializationFlags::Type flags,
					const InputDelimiter<Char, CharTraits> &separator,
					const InputDelimiter<Char, CharTraits> &lastSeparator)
				{
					DeserializeElement(is, std::get<m>(std::forward<Tuple>(t)), lastSeparator);
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
							separator,
							lastSeparator);
				}
			};

			template <std::size_t n, std::size_t m, typename Stream, typename Tuple>
				struct DeserializeTupleImpl<n, n, m, Stream, Tuple>
			{
				typedef typename Stream::char_type   Char;
				typedef typename Stream::traits_type CharTraits;

				static Stream &Apply(
					Stream &is,
					Tuple &&,
					typename SequenceDeserializationFlags::Type flags,
					const InputDelimiter<Char, CharTraits> &,
					const InputDelimiter<Char, CharTraits> &lastSeparator)
				{
					// trim trailing separators
					if (flags & SequenceDeserializationFlags::trim)
						Skip(is, lastSeparator);

					return is;
				}
			};

			template <typename Char, typename CharTraits, typename Tuple>
				std::basic_istream<Char, CharTraits> &DeserializeTuple(
					std::basic_istream<Char, CharTraits> &is,
					Tuple &&t,
					typename SequenceDeserializationFlags::Type flags,
					const InputDelimiter<Char, CharTraits> &separator,
					const InputDelimiter<Char, CharTraits> &lastSeparator)
			{
				// trim leading separators
				if ((flags & SequenceDeserializationFlags::trim) &&
					!Skip(is, separator).first)
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
						separator,
						lastSeparator);
			}
		}

		template <typename Char, typename CharTraits, typename... Types, typename Separator, typename LastSeparator>
			std::basic_istream<Char, CharTraits> &Deserialize(
				std::basic_istream<Char, CharTraits> &is,
				std::tuple<Types...> &t,
				typename SequenceDeserializationFlags::Type flags,
				Separator separator,
				LastSeparator lastSeparator)
		{
			return detail::DeserializeTuple(is, t, flags,
				InputDelimiter<Char, CharTraits>(separator),
				InputDelimiter<Char, CharTraits>(lastSeparator));
		}

		template <typename Char, typename CharTraits, typename First, typename Second, typename Separator, typename LastSeparator>
			std::basic_istream<Char, CharTraits> &Deserialize(
				std::basic_istream<Char, CharTraits> &is,
				std::pair<First, Second> &p,
				typename SequenceDeserializationFlags::Type flags,
				Separator separator,
				LastSeparator lastSeparator)
		{
			std::tuple<First, Second> tmp;

			Deserialize(is, tmp, flags,
				InputDelimiter<Char, CharTraits>(separator),
				InputDelimiter<Char, CharTraits>(lastSeparator));

			p.first  = std::get<0>(tmp);
			p.second = std::get<1>(tmp);

			return is;
		}
		///@}
		///@}

		template <typename Char, typename CharTraits, typename... Args>
			void Deserialize(std::basic_istream<Char, CharTraits> &&is, Args &&... args)
		{
			if (Deserialize(is, std::forward<Args>(args)...).fail())
				THROW((err::Exception<err::UtilModuleTag, err::SerializationTag>()))
		}
		///@}

		/**
		 * @weakgroup deserialize-string
		 * @{
		 */
		template <typename Char, typename CharTraits, typename Allocator, typename... Args>
			const std::basic_string<Char, CharTraits, Allocator> &Deserialize(
				const std::basic_string<Char, CharTraits, Allocator> &s, Args &&... args)
		{
			std::basic_istringstream<Char, CharTraits, Allocator> ss(s);
			if (Deserialize(ss, std::forward<Args>(args)...).fail())
				THROW((err::Exception<err::UtilModuleTag, err::SerializationTag>()))
			return s;
		}

		template <typename Char, typename... Args>
			const Char *Deserialize(const Char *s, Args &&... args)
		{
			std::basic_istringstream<Char> ss(s);
			if (Deserialize(ss, std::forward<Args>(args)...).fail())
				THROW((err::Exception<err::UtilModuleTag, err::SerializationTag>()))
			return s;
		}
		///@}
		///@}
	}
}
