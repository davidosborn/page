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
 *
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

#include <algorithm> // lower_bound

#include <boost/adaptors/indirected.hpp>

#include "../../err/Exception.hpp"
#include "../Node.hpp"
#include "../pipe/Pipe.hpp"
#include "Registry.hpp"

namespace page { namespace res { namespace type {

////////// Scanner /////////////////////////////////////////////////////////////

	Scanner::Scanner(const std::string &name, const ScanFunction &function, const std::vector<std::string> &extensions, const std::vector<std::string> &mimeTypes) :
		name(name), function(function), extensions(extensions), mimeTypes(mimeTypes) {}

	bool Scanner::operator ()(
		std::shared_ptr<const Pipe> const& pipe,
		ScanCallback                const& callback) const
	{
		return function(pipe, callback);
	}

////////// Record //////////////////////////////////////////////////////////////

	Record::Record(const Scanner &scanner, bool inspect, int priority) :
		scanner(scanner), inspect(inspect), priority(priority) {}

////////// Registry ////////////////////////////////////////////////////////////

	/**
	 * The lock size to use for preloading (an optimization).
	 *
	 * @note Some scanners require larger lock sizes.
	 *
	 * @note Tar puts its 8 byte format identifier at offset 257, so its
	 *       optimal lock size is 265.
	 *
	 * @note The FreeType BDF module reads in 1024 byte increments, so its
	 *       optimal lock size is 1024.
	 */
	const unsigned Registry::lockSize = 1024;

	void Registry::Register(const Record &record)
	{
		// function to compare record priority
		auto greaterPriority(
			std::bind(
				std::greater<decltype(Record::priority)>(),
				std::bind(
					util::make_member_of(&Record::priority),
					std::placeholders::_1),
				std::bind(
					util::make_member_of(&Record::priority),
					std::placeholders::_2)));

		// add record to list
		auto iter(
			records.insert(
				std::lower_bound(
					records.begin(),
					records.end(),
					record,
					greaterPriority),
				record));

		// associate extensions with record
		for (const auto &extension : record.scanner.extensions)
		{
			auto &records(extensions.insert({extension, {}}).first->second);
			records.insert(
				std::lower_bound(
					util::make_indirect_iterator(records.begin()),
					util::make_indirect_iterator(records.end()),
					record,
					greaterPriority).base(),
				iter);
		}

		// associate mime types with record
		for (const auto &mimeType : record.scanner.mimeTypes)
		{
			auto &records(mimeTypes.insert({extension, {}}).first->second);
			records.insert(
				std::lower_bound(
					util::make_indirect_iterator(records.begin()),
					util::make_indirect_iterator(records.end()),
					record,
					greaterPriority).base(),
				iter);
		}
	}

	bool Registry::Scan(const Node &node, const ScanCallback &callback) const
	{
		/* Lock the pipe for efficiency, since we're going to be repeatedly
		 * opening it and reading its header. */
		PipeLocker locker(*node.pipe, lockSize);

		// reset tried flag
		for (const auto &record : records)
			record.tried = false;

		// try scanners with matching mime type
		if (!node.mime.empty())
		{
			auto iter(mimeTypes.find(node.mime));
			if (iter != mimeTypes.end())
			{
				auto records(boost::adaptors::indirect(iter->second));
				for (const auto &record : records)
					if (record.scanner(node.pipe, callback))
						return true;

				THROW((err::Exception<err::ResModuleTag, err::NotFoundTag>("mismatched mime type") <<
					boost::errinfo_file_name(node.path) <<
					err::errinfo_subject(node.mime)))
			}
		}

		// try scanners with matching extension
		std::string extension(util::GetExtension(node.path));
		if (!extension.empty())
		{
			auto iter(extensions.find(extension));
			if (iter != extensions.end())
			{
				auto records(boost::adaptors::indirect(iter->second));
				for (const auto &record : records)
					if (record.scanner(node.pipe, callback))
						return true;

				THROW((err::Exception<err::ResModuleTag, err::NotFoundTag>("mismatched extension") <<
					boost::errinfo_file_name(node.path) <<
					err::errinfo_subject(ext)))
			}
		}

		// try all remaining scanners
		if (node.inspect)
			for (const auto &record : records)
				if (record.inspect &&
					record.scanner(node.pipe, callback))
						return true;

		return false;
	}

}}}
