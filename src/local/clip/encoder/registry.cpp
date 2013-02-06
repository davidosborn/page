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

#include <algorithm> // find, upper_bound
#include <cassert>
#include <iostream> // clog
#include <list>
#include <unordered_map>
#include <vector>

// local
#include "../../err/Exception.hpp"
#include "../../log/manip.hpp" // Warning
#include "../../util/path.hpp" // {Get,With}Extension
#include "../../util/serialize/deserialize_string.hpp" // Deserialize
#include "function.hpp" // EncoderFactory

namespace page
{
	namespace clip
	{
		namespace
		{
			struct Registry
			{
				struct Encoder
				{
					EncoderFactory factory;
					std::vector<std::string> exts;
					unsigned rank;
				};
				typedef std::list<Encoder> Encoders;
				typedef std::vector<Encoders::const_iterator> AssociatedEncoders;
				typedef std::unordered_map<std::string, AssociatedEncoders> Associations;

				Encoders encoders;
				Associations exts, fmts;
			};
			inline Registry &GetRegistry()
			{
				static Registry reg;
				return reg;
			}
		}

		/*-------+
		| access |
		+-------*/

		std::pair<EncoderFactory, boost::filesystem::path>
			GetRegisteredEncoder(
				const boost::filesystem::path &path,
				const std::string &fmt)
		{
			const Registry &reg(GetRegistry());
			// check format
			if (!fmt.empty())
			{
				auto iter(reg.fmts.find(fmt));
				if (iter != reg.fmts.end())
				{
					assert(!iter->second.empty());
					const Registry::Encoder &encoder(*iter->second.front());
					return std::make_pair(encoder.factory,
						util::WithExtension(path,
							encoder.exts.begin(),
							encoder.exts.end()));
				}
				std::clog << log::Warning << "unknown clip encoding format: " << fmt << std::endl;
			}
			// check extension
			std::string ext(util::GetExtension(path));
			if (!ext.empty())
			{
				auto iter(reg.exts.find(ext));
				if (iter != reg.exts.end())
				{
					assert(!iter->second.empty());
					const Registry::Encoder &encoder(*iter->second.front());
					return std::make_pair(encoder.factory, path);
				}
			}
			// use default encoder
			if (!reg.encoders.empty())
			{
				const Registry::Encoder &encoder(reg.encoders.front());
				return std::make_pair(encoder.factory,
					util::WithExtension(path,
						encoder.exts.begin(),
						encoder.exts.end()));
			}
			THROW((err::Exception<err::ClipModuleTag, err::NotAvailableTag>("no clip encoders available")))
		}

		/*-------------+
		| registration |
		+-------------*/

		void RegisterEncoder(
			const EncoderFactory &factory,
			const std::string &ext,
			const std::string &fmt,
			unsigned rank)
		{
			assert(factory);
			Registry &reg(GetRegistry());
			// parse extensions for encoder
			std::vector<std::string> exts;
			util::Deserialize(ext, std::back_inserter(exts), util::SequenceDeserializationFlags::none, ',');
			// register encoder
			Registry::Encoder encoder = {factory, exts, rank};
			auto encoderIter(reg.encoders.insert(
				std::lower_bound(
					reg.encoders.begin(),
					reg.encoders.end(),
					encoder,
					[](
						const Registry::Encoder &a,
						const Registry::Encoder &b) { return a.rank > b.rank; }),
				encoder));
			// register extensions
			for (const auto &ext : exts)
			{
				auto assocEncoders(reg.exts.insert(
					std::make_pair(ext, Registry::AssociatedEncoders())).first->second);
				assocEncoders.insert(
					std::lower_bound(
						assocEncoders.begin(),
						assocEncoders.end(),
						encoderIter,
						[](
							const Registry::Encoders::const_iterator &a,
							const Registry::Encoders::const_iterator &b) { return a->rank > b->rank; }),
					encoderIter);
			}
			// register formats
			std::vector<std::string> fmts;
			util::Deserialize(fmt, std::back_inserter(fmts), util::SequenceDeserializationFlags::none, ',');
			for (const auto &fmt : fmts)
			{
				auto assocEncoders(reg.fmts.insert(
					std::make_pair(fmt, Registry::AssociatedEncoders())).first->second);
				assocEncoders.insert(
					std::lower_bound(
						assocEncoders.begin(),
						assocEncoders.end(),
						encoderIter,
						[](
							const Registry::Encoders::const_iterator &a,
							const Registry::Encoders::const_iterator &b) { return a->rank > b->rank; }),
					encoderIter);
			}
		}
	}
}
