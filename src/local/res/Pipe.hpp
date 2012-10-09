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

#ifndef    page_local_res_Pipe_hpp
#   define page_local_res_Pipe_hpp

#	include <vector>
#	include "../util/StateSaver.hpp"

namespace page
{
	namespace res
	{
		class Stream;

		namespace detail { class LockStream; }

		/**
		 * Defines a conduit for streaming the data of a resource.
		 *
		 * Different kinds of pipes can be connected to define conduits for
		 * streaming a resource from many different sources, such as over the
		 * internet or from within .zip files, and for transforming the data
		 * while in transit.
		 */
		struct Pipe : util::NonCopyable
		{
			friend class detail::LockStream;

			// destructor
			virtual ~Pipe() {} // HACK: should be = default;

			// stream access
			/**
			 * Opens a stream for reading the data of the targeted resource.
			 */
			Stream *Open() const;
			/**
			 * Returns the number of bytes that are available for streaming
			 * through the pipe.
			 *
			 * @note In order to determine the number of bytes available, this
			 *       function may need to open a stream, which might be slow.
			 */
			virtual unsigned Size() const;

			// preloading
			/**
			 * @defgroup res-pipe-lock Resource pipe locking
			 *
			 * Pipes can be locked, which is an optimization to buffer the first
			 * n bytes, which will be used for streaming, to avoid opening a
			 * real stream when only the header is needed.
			 *
			 * @{
			 */
			/**
			 * Locks the pipe, preloading the header to a certain size in bytes.
			 */
			void Lock(unsigned size);
			/**
			 * Unlocks the pipe.
			 */
			void Unlock();
			///@}

			protected:
			virtual Stream *MakeStream() const = 0;

			private:
			typedef std::vector<char> LockBuffer;
			LockBuffer lockBuffer;
		};

		/**
		 * A pipe-locking scope-guard, which locks a pipe for the duration of
		 * its scope, and then unlocks it.
		 *
		 * @note Multiple instances locking the same pipe will not co-exist;
		 *       when either guard goes out of scope, it will completely unlock
		 *       this pipe.
		 *
		 * @ingroup res-pipe-lock scope-guard
		 */
		struct PipeLocker : util::StateSaver<PipeLocker>
		{
			friend class util::StateSaver<PipeLocker>;
			typedef util::StateSaver<PipeLocker> Base;

			// constructors
			PipeLocker();
			PipeLocker(Pipe &, unsigned size);

			// modifiers
			void Reset();
			void Reset(Pipe &, unsigned size);

			private:
			void Save();
			void Load();
			
			Pipe *pipe;
			unsigned size;
		};
	}
}

#endif
