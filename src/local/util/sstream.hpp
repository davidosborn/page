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

#ifndef    page_util_sstream_hpp
#   define page_util_sstream_hpp

#	include <sstream>
#	include "ios.hpp" // InitIos

namespace page
{
	namespace util
	{
		/**
		 * @defgroup sstream-factory Standard string-stream factory-functions
		 * @{
		 */
		/**
		 * @defgroup sstream-factory-istringstream Factory-functions for std::basic_istringstream
		 * @{
		 */
		template <
			typename Char,
			typename CharTraits = std::char_traits<Char>,
			typename Allocator  = std::allocator<Char>>
			inline std::basic_istringstream<Char, CharTraits, Allocator>
			make_istringstream(
				std::ios_base::openmode which = std::ios_base::in)
		{
			std::basic_istringstream<Char, CharTraits, Allocator> ss(which);
			InitIos(ss);
			return ss;
		}
		template <
			typename Char,
			typename CharTraits = std::char_traits<Char>,
			typename Allocator  = std::allocator<Char>>
			inline std::basic_istringstream<Char, CharTraits, Allocator>
			make_istringstream(
				const std::basic_string<Char, CharTraits, Allocator> &s,
				std::ios_base::openmode which = std::ios_base::in)
		{
			std::basic_istringstream<Char, CharTraits, Allocator> ss(s, which);
			InitIos(ss);
			return ss;
		}
		template <
			typename Char,
			typename CharTraits = std::char_traits<Char>,
			typename Allocator  = std::allocator<Char>>
			inline std::basic_istringstream<Char, CharTraits, Allocator>
			make_istringstream(
				const std::ios &other,
				std::ios_base::openmode which = std::ios_base::in)
		{
			std::basic_istringstream<Char, CharTraits, Allocator> ss(which);
			CopyIos(ss, other);
			return ss;
		}
		template <
			typename Char,
			typename CharTraits = std::char_traits<Char>,
			typename Allocator  = std::allocator<Char>>
			inline std::basic_istringstream<Char, CharTraits, Allocator>
			make_istringstream(
				const std::ios &other,
				const std::basic_string<Char, CharTraits, Allocator> &s,
				std::ios_base::openmode which = std::ios_base::in)
		{
			std::basic_istringstream<Char, CharTraits, Allocator> ss(s, which);
			CopyIos(ss, other);
			return ss;
		}
		///@}

		/**
		 * @defgroup sstream-factory-ostringstream Factory-functions for std::basic_ostringstream
		 * @{
		 */
		template <
			typename Char,
			typename CharTraits = std::char_traits<Char>,
			typename Allocator  = std::allocator<Char>>
			inline std::basic_ostringstream<Char, CharTraits, Allocator>
			make_ostringstream(
				std::ios_base::openmode which = std::ios_base::out)
		{
			std::basic_ostringstream<Char, CharTraits, Allocator> ss(which);
			InitIos(ss);
			return ss;
		}
		template <
			typename Char,
			typename CharTraits = std::char_traits<Char>,
			typename Allocator  = std::allocator<Char>>
			inline std::basic_ostringstream<Char, CharTraits, Allocator>
			make_ostringstream(
				const std::basic_string<Char, CharTraits, Allocator> &s,
				std::ios_base::openmode which = std::ios_base::out)
		{
			std::basic_ostringstream<Char, CharTraits, Allocator> ss(s, which);
			InitIos(ss);
			return ss;
		}
		template <
			typename Char,
			typename CharTraits = std::char_traits<Char>,
			typename Allocator  = std::allocator<Char>>
			inline std::basic_ostringstream<Char, CharTraits, Allocator>
			make_ostringstream(
				const std::ios &other,
				std::ios_base::openmode which = std::ios_base::out)
		{
			std::basic_ostringstream<Char, CharTraits, Allocator> ss(which);
			CopyIos(ss, other);
			return ss;
		}
		template <
			typename Char,
			typename CharTraits = std::char_traits<Char>,
			typename Allocator  = std::allocator<Char>>
			inline std::basic_ostringstream<Char, CharTraits, Allocator>
			make_ostringstream(
				const std::ios &other,
				const std::basic_string<Char, CharTraits, Allocator> &s,
				std::ios_base::openmode which = std::ios_base::out)
		{
			std::basic_ostringstream<Char, CharTraits, Allocator> ss(s, which);
			CopyIos(ss, other);
			return ss;
		}
		///@}

		/**
		 * @defgroup sstream-factory-stringstream Factory-functions for std::basic_stringstream
		 * @{
		 */
		template <
			typename Char,
			typename CharTraits = std::char_traits<Char>,
			typename Allocator  = std::allocator<Char>>
			inline std::basic_stringstream<Char, CharTraits, Allocator>
			make_stringstream(
				std::ios_base::openmode which = std::ios_base::in | std::ios_base::out)
		{
			std::basic_stringstream<Char, CharTraits, Allocator> ss(which);
			InitIos(ss);
			return ss;
		}
		template <
			typename Char,
			typename CharTraits = std::char_traits<Char>,
			typename Allocator  = std::allocator<Char>>
			inline std::basic_stringstream<Char, CharTraits, Allocator>
			make_stringstream(
				const std::basic_string<Char, CharTraits, Allocator> &s,
				std::ios_base::openmode which = std::ios_base::in | std::ios_base::out)
		{
			std::basic_stringstream<Char, CharTraits, Allocator> ss(s, which);
			InitIos(ss);
			return ss;
		}
		template <
			typename Char,
			typename CharTraits = std::char_traits<Char>,
			typename Allocator  = std::allocator<Char>>
			inline std::basic_stringstream<Char, CharTraits, Allocator>
			make_stringstream(
				const std::ios &other,
				std::ios_base::openmode which = std::ios_base::in | std::ios_base::out)
		{
			std::basic_stringstream<Char, CharTraits, Allocator> ss(which);
			CopyIos(ss, other);
			return ss;
		}
		template <
			typename Char,
			typename CharTraits = std::char_traits<Char>,
			typename Allocator  = std::allocator<Char>>
			inline std::basic_stringstream<Char, CharTraits, Allocator>
			make_stringstream(
				const std::ios &other,
				const std::basic_string<Char, CharTraits, Allocator> &s,
				std::ios_base::openmode which = std::ios_base::in | std::ios_base::out)
		{
			std::basic_stringstream<Char, CharTraits, Allocator> ss(s, which);
			CopyIos(ss, other);
			return ss;
		}
		///@}
		///@}
		
	/**
	 * @defgroup sstream-factory-macro Standard string-stream factory-macros
	 * @ingroup  sstream-factory
	 *
	 * Factory macros for @c std::basic_stringstream (and its variations), which
	 * are needed until @c std::stringstream becomes movable in GCC.
	 *
	 * @{
	 */
#	define MAKE_ISTRINGSTREAM_1(CHAR, S) \
		std::basic_istringstream<CHAR> ss(S); \
		util::InitIos(ss);
#	define MAKE_OSTRINGSTREAM_1(CHAR) \
		std::basic_ostringstream<CHAR> ss; \
		util::InitIos(ss);
#	define MAKE_ISTRINGSTREAM_1_COPY(CHAR, IOS, S) \
		std::basic_istringstream<CHAR> ss(S); \
		util::CopyIos(ss, IOS);
#	define MAKE_OSTRINGSTREAM_1_COPY(CHAR, IOS) \
		std::basic_ostringstream<CHAR> ss; \
		util::CopyIos(ss, IOS);

#	define MAKE_ISTRINGSTREAM_2(CHAR, CHAR_TRAITS, S) \
		std::basic_istringstream<CHAR, CHAR_TRAITS> ss(S); \
		util::InitIos(ss);
#	define MAKE_OSTRINGSTREAM_2(CHAR, CHAR_TRAITS) \
		std::basic_ostringstream<CHAR, CHAR_TRAITS> ss; \
		util::InitIos(ss);
#	define MAKE_ISTRINGSTREAM_2_COPY(CHAR, CHAR_TRAITS, IOS, S) \
		std::basic_istringstream<CHAR, CHAR_TRAITS> ss(S); \
		util::CopyIos(ss, IOS);
#	define MAKE_OSTRINGSTREAM_2_COPY(CHAR, CHAR_TRAITS, IOS) \
		std::basic_ostringstream<CHAR, CHAR_TRAITS> ss; \
		util::CopyIos(ss, IOS);

#	define MAKE_ISTRINGSTREAM_3(CHAR, CHAR_TRAITS, ALLOCATOR, S) \
		std::basic_istringstream<CHAR, CHAR_TRAITS, ALLOCATOR> ss(S); \
		util::InitIos(ss);
#	define MAKE_OSTRINGSTREAM_3(CHAR, CHAR_TRAITS, ALLOCATOR) \
		std::basic_ostringstream<CHAR, CHAR_TRAITS, ALLOCATOR> ss; \
		util::InitIos(ss);
#	define MAKE_ISTRINGSTREAM_3_COPY(CHAR, CHAR_TRAITS, ALLOCATOR, IOS, S) \
		std::basic_istringstream<CHAR, CHAR_TRAITS, ALLOCATOR> ss(S); \
		util::CopyIos(ss, IOS);
#	define MAKE_OSTRINGSTREAM_3_COPY(CHAR, CHAR_TRAITS, ALLOCATOR, IOS) \
		std::basic_ostringstream<CHAR, CHAR_TRAITS, ALLOCATOR> ss; \
		util::CopyIos(ss, IOS);
	///@}
	}
}

#endif
