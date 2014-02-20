/**
 * @copyright
 *
 * Copyright (c) 2006-2014 David Osborn
 *
 * Permission is granted to use and redistribute this software in source and
 * binary form, with or without modification, subject to the following
 * conditions:
 *
 * 1. Redistributions in source form must retain the above copyright notice,
 *    this list of conditions, and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions, and the following disclaimer in the same place
 *    and form as other copyright, license, and disclaimer information.
 *
 * 3. Redistributions in binary form must also include an acknowledgement in the
 *    same place and form as other acknowledgements (such as the credits),
 *    similar in substance to the following:
 *
 *       Portions of this software are based on the work of David Osborn.
 *
 * This software is provided "as is", without any express or implied warranty.
 * In no event will the authors be liable for any damages arising out of the use
 * of this software.
 */

// input stream
// when EndOfStream is thrown, the stream position should be set to the end
// when InStream is thrown, the stream position should not change

#ifndef    page_local_res_pipe_Stream_hpp
#   define page_local_res_pipe_Stream_hpp

#	include <string>
#	include <vector>

#	include "../util/class/special_member_functions.hpp" // Uncopyable

namespace page
{
	namespace res
	{
		struct Stream : util::Uncopyable<Stream>
		{
			enum SeekOrigin
			{
				begSeekOrigin,
				curSeekOrigin,
				endSeekOrigin
			};

			// construct/destroy
			Stream();
			virtual ~Stream();

			// formatted input
			char GetChar();
			std::string GetLine(char delim = '\n');
			std::string GetText();

			// unformatted input
			void Read(void *, unsigned);
			unsigned ReadSome(void *, unsigned);

			// positioning
			unsigned Tell() const;
			unsigned Size() const;
			void Seek(unsigned);
			void Seek(int, SeekOrigin);

			// input state
			bool Eof() const;

			protected:
			// input
			virtual void DoRead(void *, unsigned) = 0;
			virtual unsigned DoReadSome(void *, unsigned) = 0;

			// positioning
			virtual unsigned DoTell() const = 0;
			virtual unsigned DoSize() const = 0;
			virtual void DoSeek(unsigned) = 0;

			private:
			typedef std::vector<char> Buffer;
			Buffer buffer; // for formatted input
			unsigned bufferPos;
			char endl;
			bool eof;
		};
	}
}

#endif
