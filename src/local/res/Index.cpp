#include <memory> // unique_ptr
#include <iostream> // cout

#include "../cfg/vars.hpp"
#include "../err/Exception.hpp"
#include "../err/report.hpp" // ReportError, std::exception
#include "../log/Indenter.hpp"
#include "../opt.hpp" // resourceSources
#include "Index.hpp"
#include "node/path.hpp" // NormPath
#include "pipe/Stream.hpp" // Stream::GetText
#include "source/Source.hpp" // Source::{~Source,Open,Refresh}
#include "source/SourceRegistry.hpp"
#include "type/TypeRegistry.hpp"

namespace page { namespace res
{
	/*-------------+
	| constructors |
	+-------------*/

	Index::Index()
	{
		for (const auto &source : *CVAR(resourceSources)) AddSource(source);
		for (const auto &source :  opt::resourceSources) AddSource(source);
	}

	/*----------+
	| modifiers |
	+----------*/

	void Index::AddSource(const std::string &path)
	{
		// FIXME: This should go in the Source class, but it can't
		// go in the base-class constructor because the indenter
		// will be destroyed before the derived-class constructor
		// executes, which is where all the indexing happens.
		std::cout << "indexing source: " << path << std::endl;
		log::Indenter indenter;
		sources.push_front(GLOBAL(SourceRegistry).Make(path));
	}

	void Index::Refresh()
	{
		for (const auto &source : sources)
			source->Refresh();
	}

	/*----------------+
	| resource access |
	+----------------*/

	Stream *Index::Open(const std::string &path) const
	{
		std::string normPath(NormPath(path));
		for (const auto &source : sources)
		{
			Stream *stream = source->Open(normPath);
			if (stream) return stream;
		}
		THROW((err::Exception<err::ResModuleTag, err::NotFoundTag>("resource not found")))
	}

	std::shared_ptr<const void> Index::Load(const std::type_info &type, const std::string &path) const
	{
		std::string normPath(NormPath(path));
		std::cout << "loading " << GLOBAL(TypeRegistry).Query(type).name << " from " << normPath << std::endl;
		log::Indenter indenter;
		try
		{
			for (const auto &source : sources)
			{
				std::shared_ptr<const void> resource(source->Load(type, normPath));
				if (resource) return resource;
			}
			THROW((err::Exception<err::ResModuleTag, err::NotFoundTag>("resource not found")))
		}
		catch (const std::exception &e)
		{
			err::ReportError(e);
			throw;
		}
	}

	std::string Index::LoadString(const std::string &path) const
	{
		const std::unique_ptr<Stream> stream(Open(path));
		return stream->GetText();
	}
}}
