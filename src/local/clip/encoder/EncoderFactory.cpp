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

// C++
#include <algorithm> // upper_bound
#include <functional> // greater
#include <iostream> // clog
#include <utility> // forward

// Boost
#include <boost/range/adaptor/indirected.hpp> // indirect
#include <boost/range/adaptor/map.hpp> // keys
#include <boost/range/algorithm/copy.hpp>

// local
#include "../../err/Exception.hpp"
#include "../../log/manip.hpp" // Warning
#include "../../util/functional/compare.hpp" // indirect_greater
#include "../../util/io/separated_istream_iterator.hpp"
#include "../../util/path.hpp" // {Get,With}Extension
#include "EncoderFactory.hpp"

namespace page
{
	namespace clip
	{
		/*----------------+
		| global instance |
		+----------------*/

		EncoderFactory &EncoderFactory::GetGlobalInstance()
		{
			static EncoderFactory instance;
			return instance;
		}

		/*-------------+
		| registration |
		+-------------*/

		void CodecFactory<T, A...>::Register(
			const Function &function,
			int priority,
			const std::vector<std::string> &formats,
			const std::vector<std::string> &mimeTypes,
			const std::vector<std::string> &extensions)
		{
			Blueprint blueprint =
			{
				function, priority,
				formats, mimeTypes, extensions
			};

			// insert blueprint into list
			auto iter(
				blueprints.insert(
					std::upper_bound(
						blueprints.begin(),
						blueprints.end(),
						blueprint,
						std::greater<Blueprint>()),
					blueprint));

			// associate blueprint with its formats
			for (const auto &format : blueprint.formats)
			{
				auto &blueprints(formatToBlueprint[format]);
				blueprints.insert(
					std::upper_bound(
						blueprints.begin(),
						blueprints.end(),
						iter,
						util::indirect_greater<decltype(iter)>()),
					iter);
			}

			// associate blueprint with its mimeTypes
			for (const auto &mimeType : blueprint.mimeTypes)
			{
				auto &blueprints(mimeTypeToCodec[mimeType]);
				blueprints.insert(
					std::upper_bound(
						blueprints.begin(),
						blueprints.end(),
						iter,
						util::indirect_greater<decltype(iter)>()),
					iter);
			}

			// associate blueprint with its extensions
			for (const auto &extension : blueprint.extensions)
			{
				auto &blueprints(extensionToCodec[extension]);
				blueprints.insert(
					std::upper_bound(
						blueprints.begin(),
						blueprints.end(),
						iter,
						util::indirect_greater<decltype(iter)>()),
					iter);
			}
		}

		template <typename AbstractCodec, typename... ConstructorArgs>
			template <typename ConcreteCodec>
				void CodecFactory<AbstractCodec, ConstructorArgs...>::Register(
					int priority,
					const std::string &formats,
					const std::string &mimeTypes,
					const std::string &extensions)
		{
			Register(
				[](ConstructorArgs &&... args)
				{
					return std::unique_ptr<AbstractCodec>(
						new ConcreteCodec(std::forward<ConstructorArgs>(args)...));
				},
				priority,
				formats,
				mimeTypes,
				extensions);
		}

		/*-----------+
		| production |
		+-----------*/

		template <typename AbstractCodec, typename... ConstructorArgs>
			typename CodecFactory<AbstractCodec, ConstructorArgs...>::Product
				CodecFactory<AbstractCodec, ConstructorArgs...>::Make(
					const boost::filesystem::path &path,
					ConstructorArgs &&... args) const
		{
			ResetTried();

			for (const auto &codec : codecs)
				try
				{
					if (!codec.tried)
					{
						Product r =
						{
							codec.function(std::forward<ConstructorArgs>(args)...)
						};
						if (r.codec)
						{
							r.path = util::WithExtension(path,
								codec.extensions.begin(),
								codec.extensions.end());
							return r;
						}
						codec.tried = true;
					}
				}
				catch (const std::exception &) {}

			THROW((err::Exception<err::UtilModuleTag, err::FactoryTag>("factory failed to make codec")))
		}

		template <typename AbstractCodec, typename... ConstructorArgs>
			typename CodecFactory<AbstractCodec, ConstructorArgs...>::Product
				CodecFactory<AbstractCodec, ConstructorArgs...>::Make(
					const boost::filesystem::path &path,
					const std::vector<std::string> &format,
					const std::vector<std::string> &mimeType,
					ConstructorArgs &&... args) const
		{
			ResetTried();

			// try codecs with matching format
			if (!formats.empty())
				for (const auto &format : formats)
				{
					auto iter(formatToCodec.find(format));
					if (iter != formatToCodec.end())
						for (const auto &codec : boost::adaptors::indirect(iter->second))
							try
							{
								Product r =
								{
									codec.function(std::forward<ConstructorArgs>(args)...)
								};
								if (r.codec)
								{
									r.path = util::WithExtension(path,
										codec.extensions.begin(),
										codec.extensions.end());
									return r;
								}
								codec.tried = true;
							}
							catch (const std::exception &) {}

					std::clog << log::Warning << "unrecognized format: " << format << std::endl;
				}

			// try codecs with matching mime type
			if (!mimeTypes.empty())
				for (const auto &mimeType : mimeTypes)
				{
					auto iter(mimeTypeToCodec.find(mimeType));
					if (iter != mimeTypeToCodec.end())
						for (const auto &codec : boost::adaptors::indirect(iter->second))
							try
							{
								Product r =
								{
									codec.function(std::forward<ConstructorArgs>(args)...)
								};
								if (r.codec)
								{
									r.path = util::WithExtension(path,
										codec.extensions.begin(),
										codec.extensions.end());
									return r;
								}
								codec.tried = true;
							}
							catch (const std::exception &) {}

					std::clog << log::Warning << "unrecognized mime type: " << mimeType << std::endl;
				}

			// try codecs with matching extension
			std::string extension(util::GetExtension(path));
			if (!extension.empty())
			{
				auto iter(extensionToCodec.find(extension));
				if (iter != extensionToCodec.end())
					for (const auto &codec : boost::adaptors::indirect(iter->second))
						try
						{
							Product r =
							{
								codec.function(std::forward<ConstructorArgs>(args)...)
							};
							if (r.codec)
							{
								r.path = path;
								return r;
							}
							codec.tried = true;
						}
						catch (const std::exception &) {}

				std::clog << log::Warning << "unrecognized extension: " << extension << std::endl;
			}

			// try all remaining codecs
			return Make(path, std::forward<ConstructorArgs>(args)...);
		}

		/*-------------+
		| data members |
		+-------------*/

		template <typename T, typename... A>
			CodecFactory<T, A...>::ConcreteCodec::ConcreteCodec(const Function &function, int priority) :
				function(function), priority(priority) {}

		template <typename T, typename... A>
			void CodecFactory<T, A...>::ResetTried() const
		{
			for (const auto &codec : codecs)
				codec.tried = false;
		}

		/*----------+
		| selection |
		+----------*/

		template <typename T, typename... A>
			typename CodecFactory<T, A...>::Selection
				CodecFactory<T, A...>::Select(
					const std::string &formats,
					const std::string &mimeTypes,
					const std::string &extensions)
		{
		}

		/*---------+
		| ordering |
		+---------*/

		template <typename T, typename... A>
			bool operator ==(
				const typename CodecFactory<T, A...>::ConcreteCodec &a,
				const typename CodecFactory<T, A...>::ConcreteCodec &b)
		{
			return a.priority == b.priority;
		}

		template <typename T, typename... A>
			bool operator <(
				const typename CodecFactory<T, A...>::ConcreteCodec &a,
				const typename CodecFactory<T, A...>::ConcreteCodec &b)
		{
			return a.priority < b.priority;
		}
	}
}
