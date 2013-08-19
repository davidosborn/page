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
			 * Write a single character to the stream.
			 */
			void Write(char);

			/**
			 * Write an array of characters to the stream.
			 */
			void Write(const char *, unsigned);

			/**
			 * Write a string to the stream.
			 */
			void Write(const std::string &);

			/**
			 * Write a C-string to the stream.
			 */
			void Write(const char *);

			/**
			 * Flush any buffered output.
			 */
			void Flush();

			/**
			 * Clear the stream of any intermediate state.
			 */
			void Clear();

			/*------------------+
			| virtual functions |
			+------------------*/

			private:
			/**
			 * Write a string to the stream.
			 *
			 * @note Implementations are permitted to buffer the output.
			 */
			virtual void DoWrite(const std::string &) = 0;

			/**
			 * Flush any buffered output.
			 *
			 * @note The default implementation does nothing.
			 */
			virtual void DoFlush();

			/**
			 * Clear the stream of any intermediate state.
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
			 * @return @c *this up-casted to @c std::streambuf.
			 */
			std::streambuf &streambuf();

			/**
			 * @return @c *this up-casted to @c std::streambuf.
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
