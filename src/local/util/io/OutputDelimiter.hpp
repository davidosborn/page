#ifndef    page_local_util_io_OutputDelimiter_hpp
#   define page_local_util_io_OutputDelimiter_hpp

#	include <string> // basic_string, char_traits

namespace page
{
	namespace util
	{
		/**
		 * A class representing an output delimiter.
		 */
		template <typename Char, typename CharTraits = std::char_traits<Char>>
			class OutputDelimiter
		{
			public:
			typedef std::basic_string<Char, CharTraits> String;

			/*-------------+
			| constructors |
			+-------------*/

			/**
			 * Creates an empty delimiter.
			 */
			OutputDelimiter();

			/**
			 * Creates a delimiter consisting of a single character.
			 */
			OutputDelimiter(Char);

			/**
			 * Creates a delimiter consisting of a string.
			 */
			OutputDelimiter(const Char *);

			/**
			 * Creates a delimiter consisting of a string.
			 */
			OutputDelimiter(const String &);

			/*----------+
			| observers |
			+----------*/

			/**
			 * @return @copydoc string
			 */
			const String &GetString() const;

			/**
			 * Convert the delimiter to a string.
			 */
			operator String() const;

			/**
			 * @return @c true if the delimiter is not empty.
			 */
			explicit operator bool() const;

			/*----------------------+
			| predefined delimiters |
			+----------------------*/

			/**
			 * @return The predefined empty-delimiter, which is empty.
			 */
			static const OutputDelimiter &GetEmptyDelimiter();

			/**
			 * @return The predefined whitespace-delimiter, which consists of a
			 *         single space character.
			 */
			static const OutputDelimiter &GetSpaceDelimiter();

			/*-------------+
			| data members |
			+-------------*/

			private:
			/**
			 * The string which is used as the delimiter.
			 */
			String string;
		};

		/*----------------------------+
		| stream insertion/extraction |
		+----------------------------*/

		/**
		 * Writes the delimiter to a stream.
		 */
		template <typename Char, typename CharTraits>
			std::basic_ostream<Char, CharTraits> &operator <<(std::basic_ostream<Char, CharTraits> &, const OutputDelimiter<Char, CharTraits> &);
	}
}

#	include "OutputDelimiter.tpp"
#endif
