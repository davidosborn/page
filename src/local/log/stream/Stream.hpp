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

#ifndef    page_local_log_stream_Stream_hpp
#   define page_local_log_stream_Stream_hpp

#	include <streambuf>
#	include <string>

namespace page
{
	namespace log
	{
		/**
		 * A streaming interface for filters and sinks, which is compatible with
		 * the Standard Library.
		 *
		 * @note This class inherits privately from @c std::streambuf to prevent
		 *       derived classes from further overriding its virtual functions.
		 */
		class Stream : std::streambuf
		{
			/*----------+
			| interface |
			+----------*/

			public:
			/**
			 * Writes a single character to the stream.
			 */
			void Write(char);

			/**
			 * Writes an array of characters to the stream.
			 */
			void Write(const char *, unsigned);

			/**
			 * Writes a string to the stream.
			 */
			void Write(const std::string &);

			/**
			 * Writes a C-string to the stream.
			 */
			void Write(const char *);

			/**
			 * Flushes any buffered output.
			 */
			void Flush();

			/**
			 * Clears the stream of any intermediate state.
			 */
			void Clear();

			/*------------------+
			| virtual functions |
			+------------------*/

			private:
			/**
			 * Writes a string to the stream.
			 *
			 * @note Implementations are permitted to buffer the output.
			 */
			virtual void DoWrite(const std::string &) = 0;

			/**
			 * Flushes any buffered output.
			 *
			 * @note The default implementation does nothing.
			 */
			virtual void DoFlush();

			/**
			 * Clears the stream of any intermediate state.
			 *
			 * @note The default implementation does nothing.
			 */
			virtual void DoClear();

			////////////////////////////////////////////////////////////////////

			/*--------------------------+
			| std::streambuf conversion |
			+--------------------------*/

			public:
			/**
			 * Returns @c *this up-casted to @c std::streambuf.
			 */
			std::streambuf &streambuf();

			/**
			 * Returns @c *this up-casted to @c std::streambuf.
			 */
			const std::streambuf &streambuf() const;

			/*------------------------------+
			| std::streambuf implementation |
			+------------------------------*/

			private:
			int sync() override final;
			std::streamsize xsputn(const char *, std::streamsize) override final;
			std::streambuf::int_type overflow(std::streambuf::int_type) override final;
		};
	}
}

#endif
