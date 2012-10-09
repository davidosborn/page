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

#ifndef    page_util_ios_hpp
#   define page_util_ios_hpp

#	include <ios>
#	include "StateSaver.hpp"

namespace page
{
	namespace util
	{
		/**
		 * @defgroup ios-format-saver Standard I/O format savers
		 *
		 * Format savers for @c std::basic_ios and @c std::ios_base.
		 *
		 * @note Inspired by Daryle Walker's boost/io/ios_state library.
		 *
		 * @ingroup scope-guard
		 *
		 * @{
		 */
		/**
		 * A format-saving scope-guard for @c std::ios_base.
		 */
		struct IosBaseFormatSaver : StateSaver<IosBaseFormatSaver>
		{
			friend class StateSaver<IosBaseFormatSaver>;
			typedef StateSaver<IosBaseFormatSaver> Base;

			// constructors
			IosBaseFormatSaver();
			explicit IosBaseFormatSaver(std::ios_base &);

			// modifiers
			void Reset();
			void Reset(std::ios_base &);

			private:
			void Save();
			void Load();

			std::ios_base          *s;
			std::ios_base::fmtflags flags;
			std::streamsize         precision;
			std::streamsize         width;
		};
		/**
		 * A format-saving scope-guard for @c std::basic_ios.
		 */
		template <typename Char, typename CharTraits = std::char_traits<Char>>
			struct BasicIosFormatSaver : StateSaver<BasicIosFormatSaver<Char, CharTraits>>
		{
			friend class StateSaver<BasicIosFormatSaver>;
			typedef StateSaver<BasicIosFormatSaver> Base;

			// constructors
			BasicIosFormatSaver();
			explicit BasicIosFormatSaver(std::ios &);

			// modifiers
			void Reset();
			void Reset(std::ios &);

			private:
			void Save();
			void Load();

			std::basic_ios<Char, CharTraits> *s;
			Char                              fill;
			IosBaseFormatSaver                iosBaseFormatSaver;
		};
		///@}

		// format initialization
		template <typename Char, typename CharTraits> void InitIos(std::basic_ios<Char, CharTraits> &);
		template <typename Char, typename CharTraits> void CopyIos(std::basic_ios<Char, CharTraits> &, const std::basic_ios<Char, CharTraits> &);
	}
}

#	include "ios.tpp"
#endif
