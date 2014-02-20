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

#include "../../util/class/special_member_functions.hpp" // Uncopyable
#include "../CmdlineParser.hpp"
#include "CmdlineSource.hpp"

namespace page { namespace cfg
{
////////// CmdlineSource::Reader ///////////////////////////////////////////////

	/**
	 * The implementation of @c CmdlineSource's reader.
	 */
	class CmdlineSource::Reader :
		public Source::Reader,
		public util::Uncopyable<CmdlineSource::Reader>
	{
		/*-------------+
		| constructors |
		+-------------*/

		public:
		explicit Reader(const CmdlineSource &);

		/*------------------------------+
		| Source::Reader implementation |
		+------------------------------*/

		public:
		boost::optional<std::string> Read(const std::string &) override;

		/*-----------------------------------------+
		| Source::ReaderWriter covariant overrides |
		+-----------------------------------------*/

		public:
		const CmdlineSource &GetSource() const;
	};

////////// CmdlineSource ///////////////////////////////////////////////////////

	/*-------------+
	| constructors |
	+-------------*/

	CmdlineSource::CmdlineSource(int argc, const char *const *argv) :
		Source("cmdline://", "command line") {}

	/*----------------------+
	| Source implementation |
	+----------------------*/

	std::unique_ptr<Source::Reader> CmdlineSource::OpenReader() const
	{
		return std::unique_ptr<Source::Reader>(new CmdlineSource::Reader(*this));
	}

////////// CmdlineSource::Reader ///////////////////////////////////////////////

	/*-------------+
	| constructors |
	+-------------*/

	CmdlineSource::Reader::Reader(const CmdlineSource &source) :
		Source::Reader(source) {}

	/*------------------------------+
	| Source::Reader implementation |
	+------------------------------*/

	boost::optional<std::string> CmdlineSource::Reader::Read(const std::string &key)
	{
		for (const auto &kv : GLOBAL(CmdlineParser).GetCfgVars())
			if (kv.first == key)
				return kv.second;
		return boost::none;
	}

	/*-----------------------------------------+
	| Source::ReaderWriter covariant overrides |
	+-----------------------------------------*/

	const CmdlineSource &CmdlineSource::Reader::GetSource() const
	{
		return static_cast<const CmdlineSource &>(Source::Reader::GetSource());
	}
}}
