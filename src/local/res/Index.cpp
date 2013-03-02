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

#include <memory> // unique_ptr
#include <iostream> // cout

#include "../cfg/vars.hpp"
#include "../err/Exception.hpp"
#include "../err/report.hpp" // ReportError, std::exception
#include "../log/Indenter.hpp"
#include "../opt.hpp" // resourceSources
#include "Index.hpp"
#include "path.hpp" // NormPath
#include "source/registry.hpp" // MakeSource
#include "source/Source.hpp" // Source::{~Source,Open,Refresh}
#include "Stream.hpp" // Stream::GetText
#include "type/Registry.hpp"

namespace page
{
	namespace res
	{
		// constructors
		Index::Index()
		{
			for (const auto &source : *CVAR(resourceSources)) AddSource(source);
			for (const auto &source :  opt::resourceSources) AddSource(source);
		}

		// modifiers
		void Index::AddSource(const std::string &path)
		{
			// FIXME: This should go in the Source class, but it can't
			// go in the base-class constructor because the indenter
			// will be destroyed before the derived-class constructor
			// executes, which is where all the indexing happens.
			std::cout << "indexing source: " << path << std::endl;
			log::Indenter indenter;
			sources.push_front(std::shared_ptr<Source>(MakeSource(path)));
		}
		void Index::Refresh()
		{
			for (const auto &source : sources)
				source->Refresh();
		}

		// resource access
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
			std::cout << "loading " << GLOBAL(type::Registry).Query(type).name << " from " << normPath << std::endl;
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

		// global state
		Index &GetIndex()
		{
			static Index index;
			return index;
		}
	}
}
