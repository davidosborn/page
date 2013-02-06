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
#include <algorithm> // copy, transform, upper_bound
#include <functional> // greater
#include <iostream> // clog
#include <iterator> // back_inserter
#include <utility> // forward

// Boost
#include <boost/range/adaptor/indirected.hpp> // indirect
#include <boost/range/adaptor/map.hpp> // keys
#include <boost/range/algorithm/copy.hpp>

// local
#include "../err/Exception.hpp"
#include "../log/manip.hpp" // Warning
#include "functional/compare.hpp" // indirect_less
#include "io/separated_istream_iterator.hpp"
#include "path.hpp" // {Get,With}Extension

namespace page
{
	namespace util
	{
		/*----------------+
		| global instance |
		+----------------*/

		template <typename T, typename... A>
			CodecFactory<T, A...> &CodecFactory<T, A...>::GetGlobalInstance()
		{
			static CodecFactory instance;
			return instance;
		}

		/*----------+
		| observers |
		+----------*/

		template <typename T, typename... A>
			std::vector<std::string> CodecFactory<T, A...>::GetFormats() const
		{
			// FIXME: this implementation is supposed to be ordered by priority

			std::vector<std::string> formats;
			formats.reserve(formatToCodec.size());
			boost::copy(
				boost::adaptors::keys(formatToCodec),
				std::back_inserter(formats));
			return formats;
		}

		template <typename T, typename... A>
			std::vector<std::string> CodecFactory<T, A...>::GetMimeTypes() const
		{
			// FIXME: this implementation is supposed to be ordered by priority

			std::vector<std::string> mimeTypes;
			mimeTypes.reserve(mimeTypeToCodec.size());
			boost::copy(
				boost::adaptors::keys(mimeTypeToCodec),
				std::back_inserter(mimeTypes));
			return mimeTypes;
		}

		template <typename T, typename... A>
			std::vector<std::string> CodecFactory<T, A...>::GetExtensions() const
		{
			// FIXME: this implementation is really ugly and probably slow

			std::list<typename decltype(extensionToCodec)::value_type> extensions;
			extensions.reserve(extensionToCodec.size());
			for (const auto &kv : extensionToCodec)
			{
				extensions.insert(
					std::upper_bound(
						extensions.begin(),
						extensions.end(),
						kv,
						[](
							const typename decltype(extensionToCodec)::value_type &a,
							const typename decltype(extensionToCodec)::value_type &b)
						{
							return
								a.second.front().priority >
								b.second.front().priority;
						}),
					kv);
			}
			std::vector<std::string> extensions2;
			boost::copy(
				boost::adaptors::keys(extensions),
				std::back_inserter(extensions2));
			return extensions2;
		}

		/*-------------+
		| registration |
		+-------------*/

		template <typename T, typename... A>
			void CodecFactory<T, A...>::Register(
				const Function &function,
				const std::string &formats,
				const std::string &mimeTypes,
				const std::string &extensions,
				int priority)
		{
			ConcreteCodec codec(function, priority);

			// extract formats from string
			{
				std::istringstream ss(formats);
				std::copy(
					util::separated_istream_iterator<std::string>(ss, ','),
					util::separated_istream_iterator<std::string>(),
					std::back_inserter(codec.formats));
			}

			// extract mime types from string
			{
				std::istringstream ss(mimeTypes);
				std::copy(
					util::separated_istream_iterator<std::string>(ss, ','),
					util::separated_istream_iterator<std::string>(),
					std::back_inserter(codec.mimeTypes));
			}

			// extract extensions from string
			{
				std::istringstream ss(extensions);
				std::copy(
					util::separated_istream_iterator<std::string>(ss, ','),
					util::separated_istream_iterator<std::string>(),
					std::back_inserter(codec.extensions));
			}

			// insert codec into list
			auto iter(
				codecs.insert(
					std::upper_bound(
						codecs.begin(),
						codecs.end(),
						codec,
						std::greater<ConcreteCodec>()),
					codec));

			// associate codec with its formats
			for (const auto &format : codec.formats)
			{
				auto &codecs(formatToCodec[format]);
				codecs.insert(
					std::upper_bound(
						codecs.begin(),
						codecs.end(),
						iter,
						util::indirect_less<decltype(iter)>()),
					iter);
			}

			// associate codec with its mimeTypes
			for (const auto &mimeType : codec.mimeTypes)
			{
				auto &codecs(mimeTypeToCodec[mimeType]);
				codecs.insert(
					std::upper_bound(
						codecs.begin(),
						codecs.end(),
						iter,
						[](const decltype(iter) &a, const decltype(iter) &b)
						{
							return *a > *b;
						}),
					iter);
			}

			// associate codec with its extensions
			for (const auto &extension : codec.extensions)
			{
				auto &codecs(extensionToCodec[extension]);
				codecs.insert(
					std::upper_bound(
						codecs.begin(),
						codecs.end(),
						iter,
						[](const decltype(iter) &a, const decltype(iter) &b)
						{
							return *a > *b;
						}),
					iter);
			}
		}

		template <typename AbstractCodec, typename... ConstructorArgs>
			template <typename ConcreteCodec>
				void CodecFactory<AbstractCodec, ConstructorArgs...>::Register(
					const std::string &formats,
					const std::string &mimeTypes,
					const std::string &extensions,
					int priority)
		{
			Register(
				[](ConstructorArgs &&... args)
				{
					return std::unique_ptr<AbstractCodec>(
						new ConcreteCodec(std::forward<ConstructorArgs>(args)...));
				},
				formats,
				mimeTypes,
				extensions,
				priority);
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
							ResetTried();
							return r;
						}
						codec.tried = true;
					}
				}
				catch (const std::exception &) {}

			ResetTried();
			THROW((err::Exception<err::UtilModuleTag, err::FactoryTag>("factory failed to make codec")))
		}

		template <typename AbstractCodec, typename... ConstructorArgs>
			typename CodecFactory<AbstractCodec, ConstructorArgs...>::Product
				CodecFactory<AbstractCodec, ConstructorArgs...>::Make(
					const boost::filesystem::path &path,
					const std::string &format,
					const std::string &mimeType,
					ConstructorArgs &&... args) const
		{
			// try codecs with matching format
			if (!format.empty())
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
								ResetTried();
								return r;
							}
							codec.tried = true;
						}
						catch (const std::exception &) {}

				std::clog << log::Warning << "unrecognized format: " << format << std::endl;
			}

			// try codecs with matching mime type
			if (!mimeType.empty())
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
								ResetTried();
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
								ResetTried();
								return r;
							}
							codec.tried = true;
						}
						catch (const std::exception &) {}

				std::clog << log::Warning << "unrecognized extension: " << extension << std::endl;
			}

			// try all registered codecs
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
