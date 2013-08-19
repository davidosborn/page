#include <cassert>

#include "BranchableStream.hpp"

namespace page
{
	namespace log
	{
		/*---------+
		| chaining |
		+---------*/

		void BranchableStream::Attach(const std::shared_ptr<Stream> &branch)
		{
			assert(branch);
			branches.push_back(branch);
		}

		void BranchableStream::Detach()
		{
			branches.clear();
		}

		void BranchableStream::Extend(const BranchableStream &other)
		{
			branches.insert(branches.end(), other.branches.begin(), other.branches.end());
		}

		/*----------------------+
		| Stream implementation |
		+----------------------*/

		void BranchableStream::DoWrite(const std::string &s)
		{
			for (auto &branch : branches)
				branch->Write(s);
		}

		void BranchableStream::DoFlush()
		{
			for (auto &branch : branches)
				branch->Flush();
		}

		void BranchableStream::DoClear()
		{
			for (auto &branch : branches)
				branch->Clear();
		}
	}
}
