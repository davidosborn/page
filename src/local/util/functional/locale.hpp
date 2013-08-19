#ifndef    page_local_util_functional_locale_hpp
#   define page_local_util_functional_locale_hpp

#	include <locale> // isspace, locale, to{lower,upper}

namespace page
{
	namespace util
	{
		/**
		 * @defgroup functional-locale
		 *
		 * Function objects for the localization and character-classification
		 * functions from the @c <locale> and @c <cctype> headers.
		 *
		 * @note Lambda expressions should generally be preferred to these.
		 *
		 * @{
		 */
		template <typename Char>
			struct isnewline_function
		{
			bool operator ()(Char c) const
			{
				switch (c)
				{
					case '\n':
					case '\r': return true;
					default:   return false;
				}
			}
		};

		template <typename Char>
			struct isspace_function
		{
			bool operator ()(Char c) const
			{
				return std::isspace(c, std::locale());
			}
		};

		template <typename Char>
			struct tolower_function
		{
			bool operator ()(Char c) const
			{
				return std::tolower(c, std::locale());
			}
		};

		template <typename Char>
			struct toupper_function
		{
			bool operator ()(Char c) const
			{
				return std::toupper(c, std::locale());
			}
		};
		///@}
	}
}

#endif
