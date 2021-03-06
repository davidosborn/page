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
