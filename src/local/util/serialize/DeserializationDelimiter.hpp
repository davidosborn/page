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

#ifndef    page_local_util_serialize_DeserializationDelimiter_hpp
#   define page_local_util_serialize_DeserializationDelimiter_hpp

#	include <functional> // function
#	include <string> // char_traits
#	include <type_traits> // is_{,explicitly_}convertible
#	include "../functional.hpp" // isspace_function
#	include "../type_traits.hpp" // ENABLE_IF, is_{{,convertible_to_}string,range}

namespace page
{
	namespace util
	{
		/**
		 * The standard deserialization delimiter.
		 */
		template <typename Char, typename CharTraits = std::char_traits<Char>>
			struct DeserializationDelimiter
		{
			typedef Char         char_type;
			typedef CharTraits traits_type;

			/**
			 * The type of the deserialization delimiter.
			 *
			 * This is a function taking a character and returning @c bool,
			 * the value of which will be @c true if the character is a
			 * delimiter.
			 */
			typedef std::function<bool (Char)> Type;

			/**
			 * @defgroup deserialize-delimiter-normalize Delimiter normalization
			 *
			 * A function with multiple SFINAE overloads to convert other
			 * types of delimiters into the standard delimiter type.
			 *
			 * @{
			 */
			/**
			 * A no-op overload for the @c Normalize function.
			 */
			static Type Normalize(const Type &);
			
			/**
			 * A function to convert a single character into the standard
			 * delimiter type.
			 */
			static Type Normalize(Char);

			/**
			 * A function to convert a range of characters into the standard
			 * delimiter type.
			 */
			template <typename Range>
				static Type Normalize(const Range &,
					ENABLE_IF(
						is_range<Range>::value &&
						!is_string<Range>::value));

			/**
			 * A function to convert a string into the standard delimiter type.
			 */
			template <typename String>
				static Type Normalize(const String &,
					ENABLE_IF((is_string_2<String, Char, CharTraits>::value)));

			/**
			 * A function to convert a string into the standard delimiter type,
			 * where the character encoding of the string is different from that
			 * of the delimiter.
			 */
			template <typename String>
				static Type Normalize(const String &,
					ENABLE_IF((
						is_string<String>::value &&
						!is_string_2<String, Char, CharTraits>::value)));

			/**
			 * A function to convert anything like a string into the standard
			 * delimiter type.
			 */
			template <typename String>
				static Type Normalize(const String &,
					ENABLE_IF((
						!is_range<String>::value &&
						is_convertible_to_string<String>::value)));

			/**
			 * A function to convert any other compatible type into the standard
			 * delimiter type.
			 */
			template <typename Other>
				static Type Normalize(const Other &,
					ENABLE_IF((
						/**
						 * @hack Since almost anything seems to be explicitly
						 *       convertible to @c std::function, we have to
						 *       specifically disable this overload for any
						 *       types that are handled by the other overloads
						 *       or we'll have ambiguities.
						 */
						!is_range<Other>::value &&
						!is_convertible_to_string<Other>::value &&
						!std::is_convertible<Other, Type>::value &&
						std::is_explicitly_convertible<Other, Type>::value)));
			///@}

			/**
			 * @defgroup deserialize-delimiter-predefined Predefined delimiters
			 * @{
			 */
			/**
			 * A null delimiter, which always returns @c false.
			 */
			static Type None()
			{
				return [](Char) { return false; };
			}
			/**
			 * A space delimiter, which matches all whitespace characters.
			 */
			static Type Space()
			{
				return isspace_function<Char>();
			}
			///@}
		};
	}
}

#	include "DeserializationDelimiter.tpp"
#endif
