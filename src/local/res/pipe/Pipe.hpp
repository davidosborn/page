#ifndef    page_local_res_pipe_Pipe_hpp
#   define page_local_res_pipe_Pipe_hpp

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
		struct Pipe : util::Uncopyable<Pipe>
		{
			friend class detail::LockStream;

			// destructor
			virtual ~Pipe() {} // HACK: should be = default;

			// stream access
			/**
			 * Open a stream for reading the data of the targeted resource.
			 */
			Stream *Open() const;
			/**
			 * Return the number of bytes that are available for streaming
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
			 * Lock the pipe, preloading the header to a certain size in bytes.
			 */
			void Lock(unsigned size);
			/**
			 * Unlock the pipe.
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
