#ifndef    page_local_log_stream_BranchableStream_hpp
#   define page_local_log_stream_BranchableStream_hpp

#	include <memory> // shared_ptr
#	include <vector>

#	include "Stream.hpp"

namespace page
{
	namespace log
	{
		/**
		 * A stream which can have other streams attached (referred to as
		 * branches), and which will pass any data written to it onto those
		 * streams.
		 */
		class BranchableStream : public Stream
		{
			/*-------------+
			| constructors |
			+-------------*/

			public:
			/**
			 * Construct an instance with any number of branches.
			 */
			template <typename... Branches>
				explicit BranchableStream(Branches &&...);

			/*----------+
			| branching |
			+----------*/

			public:
			/**
			 * Attach a stream as a new branch.
			 */
			void Attach(const std::shared_ptr<Stream> &);

			/**
			 * Remove all branches.
			 */
			void Detach();

			/**
			 * Attach all of the branches from another @c BranchableStream.
			 */
			void Extend(const BranchableStream &);

			/*----------------------+
			| Stream implementation |
			+----------------------*/

			protected:
			void DoWrite(const std::string &) override;
			void DoFlush() override;
			void DoClear() override;

			/*-------------+
			| data members |
			+-------------*/

			private:
			std::vector<std::shared_ptr<Stream>> branches;
		};
	}
}

#	include "BranchableStream.tpp"
#endif
