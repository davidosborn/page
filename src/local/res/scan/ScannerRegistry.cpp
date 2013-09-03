#include <algorithm> // lower_bound
#include <cassert>
#include <functional> // bind, greater

#include <boost/range/adaptor/indirected.hpp>

#include "../../err/Exception.hpp"
#include "../../util/functional/member.hpp" // member_of
#include "../../util/iterator/indirect_iterator.hpp"
#include "../../util/path/extension.hpp" // GetExtension
#include "../../util/string/operations.hpp" // ToLower
#include "../node/Node.hpp"
#include "../pipe/Pipe.hpp" // PipeLocker
#include "ScannerRegistry.hpp"

namespace page { namespace res
{
////////// ScannerRegistryRecord ///////////////////////////////////////////////

	ScannerRegistryRecord::ScannerRegistryRecord(
		std::string              const& name,
		Scanner                  const& scanner,
		std::vector<std::string> const& mimeTypes,
		std::vector<std::string> const& extensions,
		bool                            inspect,
		int                             priority) :
			name(name),
			scanner(scanner),
			mimeTypes(mimeTypes),
			extensions(extensions),
			inspect(inspect),
			priority(priority)
	{
		assert(scanner != nullptr);
		assert(!mimeTypes.empty() || !extensions.empty() || inspect);

		// ensure mime types are lowercase
		for (const auto &mimeType : mimeTypes)
			assert(mimeType == util::ToLower(mimeTypes));

		// ensure extensions are lowercase
		for (const auto &extension : extensions)
			assert(extension == util::ToLower(extension));
	}

////////// ScannerRegistry /////////////////////////////////////////////////////

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
	const unsigned ScannerRegistry::lockSize = 1024;

	void ScannerRegistry::Register(const Record &record)
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

		// associate mime types with record
		for (const auto &mimeType : record.mimeTypes)
		{
			auto &records(mimeTypes.insert({mimeType, {}}).first->second);
			records.insert(
				std::lower_bound(
					util::make_indirect_iterator(records.begin()),
					util::make_indirect_iterator(records.end()),
					record,
					greaterPriority).base(),
				iter);
		}

		// associate extensions with record
		for (const auto &extension : record.extensions)
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
	}

	bool ScannerRegistry::Scan(const Node &node, const ScanCallback &callback) const
	{
		/* Lock the pipe for efficiency, since we're going to be repeatedly
		 * opening it and reading its header. */
		PipeLocker locker(*node.pipe, lockSize);

		// reset tried flag
		for (const auto &record : records)
			record.tried = false;

		// try scanners with matching mime type and extension
		auto extension(util::ToLower(util::GetExtension(node.path)));
		if (!node.mime.empty() && !extension.empty())
		{
			// FIXME: implement
		}

		// try scanners with matching mime type
		if (!node.mime.empty())
		{
			auto iter(mimeTypes.find(node.mime));
			if (iter != mimeTypes.end())
			{
				auto records(boost::adaptors::indirect(iter->second));
				for (const auto &record : records)
					if (!record.tried &&
						(record.tried = true) &&
						record.scanner(node.pipe, callback))
							return true;

				THROW((err::Exception<err::ResModuleTag, err::NotFoundTag>("mismatched mime type") <<
					boost::errinfo_file_name(node.path) <<
					err::errinfo_subject(node.mime)))
			}
		}

		// try scanners with matching extension
		if (!extension.empty())
		{
			auto iter(extensions.find(extension));
			if (iter != extensions.end())
			{
				auto records(boost::adaptors::indirect(iter->second));
				for (const auto &record : records)
					if (!record.tried &&
						(record.tried = true) &&
						record.scanner(node.pipe, callback))
							return true;

				THROW((err::Exception<err::ResModuleTag, err::NotFoundTag>("mismatched extension") <<
					boost::errinfo_file_name(node.path) <<
					err::errinfo_subject(extension)))
			}
		}

		// try all remaining scanners
		if (node.inspect)
			for (const auto &record : records)
				if (record.inspect &&
					!record.tried &&
					(record.tried = true) &&
					record.scanner(node.pipe, callback))
						return true;

		return false;
	}
}}
