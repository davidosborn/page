#include <algorithm> // lower_bound
#include <cassert>
#include <functional> // bind, greater

#include <boost/adaptors/indirected.hpp>

#include "../../err/Exception.hpp"
#include "../../util/functional/member.hpp" // member_of
#include "../../util/iterator/indirect_iterator.hpp"
#include "../../util/path/extension.hpp" // GetExtension
#include "../../util/string/operations.hpp" // ToLower
#include "../node/Node.hpp"
#include "LoaderRegistry.hpp"

namespace page { namespace res
{
////////// LoaderRegistryRecord ////////////////////////////////////////////////

	LoaderRegistryRecord::LoaderRegistryRecord(
		std::string              const& name,
		Loader                   const& loader,
		LoaderCompatibleFunction const& compatibleFunction,
		std::vector<std::string> const& mimeTypes,
		std::vector<std::string> const& extensions,
		bool                            inspect,
		int                             priority) :
			name(name),
			loader(loader),
			compatibleFunction(compatibleFunction),
			mimeTypes(mimeTypes),
			extensions(extensions),
			inspect(inspect),
			priority(priority)
	{
		assert(loader             != nullptr);
		assert(compatibleFunction != nullptr);
		assert(!mimeTypes.empty() || !extensions.empty() || inspect);

		// ensure mime types are lowercase
		for (const auto &mimeType : mimeTypes)
			assert(mimeType == util::ToLower(mimeTypes));

		// ensure extensions are lowercase
		for (const auto &extension : extensions)
			assert(extension == util::ToLower(extension));
	}

////////// LoaderRegistry //////////////////////////////////////////////////////

	void LoaderRegistry::Register(const std::type_info &type, const Record &record)
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

		// get type record for type
		auto &typeRecord(types.insert({type, {}}).first->second);

		// add record to list
		auto iter(
			typeRecord.records.insert(
				std::lower_bound(
					typeRecord.records.begin(),
					typeRecord.records.end(),
					record,
					greaterPriority),
				record));

		// associate mime types with record
		for (const auto &mimeType : record.mimeTypes)
		{
			auto &records(typeRecord.mimeTypes.insert({mimeType, {}}).first->second);
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
			auto &records(typeRecord.extensions.insert({extension, {}}).first->second);
			records.insert(
				std::lower_bound(
					util::make_indirect_iterator(records.begin()),
					util::make_indirect_iterator(records.end()),
					record,
					greaterPriority).base(),
				iter);
		}
	}

	const Loader &LoaderRegistry::GetLoader(const std::type_info &type, const Node &node) const
	{
		auto iter(types.find(type));
		if (iter != types.end())
		{
			const auto &typeRecord(iter->second);

			// reset tried flag
			for (const auto &record : typeRecord.records)
				record.tried = false;

			// try loaders with matching mime type and extension
			auto extension(util::ToLower(util::GetExtension(node.path)));
			if (!node.mime.empty() && !extension.empty())
			{
				// FIXME: implement
			}

			// try loaders with matching mime type
			if (!node.mime.empty())
			{
				auto iter(typeRecord.mimeTypes.find(node.mime));
				if (iter != typeRecord.mimeTypes.end())
				{
					auto records(boost::adaptors::indirect(iter->second));
					for (const auto &record : records)
						if (!record.tried &&
							(record.tried = true) &&
							record.compatibleFunction(*node.pipe))
								return record.loader;

					THROW((err::Exception<err::ResModuleTag, err::NotFoundTag>("mismatched mime type") <<
						boost::errinfo_file_name(node.path) <<
						err::errinfo_subject(node.mime)))
				}
			}

			// try loaders with matching extension
			if (!extension.empty())
			{
				auto iter(typeRecord.extensions.find(extension));
				if (iter != typeRecord.extensions.end())
				{
					auto records(boost::adaptors::indirect(iter->second));
					for (const auto &record : records)
						if (!record.tried &&
							(record.tried = true) &&
							record.compatibleFunction(*node.pipe))
								return record.loader;

					THROW((err::Exception<err::ResModuleTag, err::NotFoundTag>("mismatched extension") <<
						boost::errinfo_file_name(node.path) <<
						err::errinfo_subject(extension)))
				}
			}

			// try all remaining loaders
			if (node.inspect)
				for (const auto &record : typeRecord.records)
					if (record.inspect &&
						!record.tried &&
						(record.tried = true) &&
						record.compatibleFunction(*node.pipe))
							return record.loader;
		}

		return nullptr;
	}
}}
