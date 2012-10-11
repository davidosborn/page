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

#ifndef    page_local_log_stream_BufferStream_hpp
#   define page_local_log_stream_BufferStream_hpp

#	include "../../util/class.hpp" // INHERIT_CONSTRUCTORS
#	include "BranchableStream.hpp"

namespace page
{
	namespace log
	{
		/**
		 * This stream stores all data written to it until it is flushed.
		 *
		 * @note Be careful not to insert this stream too early in the chain
		 *       because it will prevent data from reaching time-sensitive
		 *       filters like @c IndentFilter and @c TimeFilter until it is
		 *       flushed.
		 */
		class BufferStream : public BranchableStream
		{
			/*--------------------------+
			| constructors & destructor |
			+--------------------------*/

			public:
			INHERIT_CONSTRUCTORS(BufferStream, BranchableStream)

			/*----------------------+
			| Stream implementation |
			+----------------------*/

			private:
			void DoWrite(const std::string &) override;
			void DoFlush() override;

			/*-------------+
			| data members |
			+-------------*/

			private:
			std::string buffer;
		};
	}
}

#endif
