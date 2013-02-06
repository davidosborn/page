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

// C++
#include <algorithm> // transform, upper_bound
#include <functional> // greater
#include <utility> // forward

// Boost
#include <boost/range/adaptor/map.hpp> // keys
#include <boost/range/algorithm/copy.hpp>

// local
#include "../../err/Exception.hpp"
#include "EncoderFactory.hpp"

namespace page
{
	namespace clip
	{
		/*----------------+
		| global instance |
		+----------------*/

		template <typename T, typename... Args>
			auto Factory<T, Args...>::GetGlobalInstance() -> Factory &
		{
			static Factory instance;
			return instance;
		}

		/*----------+
		| observers |
		+----------*/

		std::vector<std::string> EncoderFactory::GetFormats() const
		{
			// FIXME: this implementation is supposed to be ordered by priority
			
			std::vector<std::string> formats;
			formats.reserve(formatToEncoder.size());
			boost::copy(
				boost::adaptors::keys(formatToEncoder),
				std::back_inserter(formats));
			return formats;
		}

		std::vector<std::string> EncoderFactory::GetExtensions() const
		{
			// FIXME: this implementation is really ugly and probably slow
			
			std::list<typename decltype(extensionToEncoder)::value_type> extensions;
			extensions.reserve(extensionToEncoder.size());
			for (const auto &kv : extensionToEncoder)
			{
				extensions.insert(
					std::upper_bound(
						extensions.begin(),
						extensions.end(),
						kv,
						[](
							const typename decltype(extensionToEncoder)::value_type &a,
							const typename decltype(extensionToEncoder)::value_type &b)
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

		void EncoderFactory::Register(
			const Function &function,
			const std::string &formats,
			const std::string &extensions,
			int priority)
		{
			Encoder encoder =
			{
				function,
				{},
				{},
				priority
			};

			// extract formats from string
			{
				std::istringstream ss(formats);
				std::copy(
					util::separated_istream_iterator<std::string>(ss, ','),
					util::separated_istream_iterator<std::string>(),
					std::back_inserter(encoder.formats));
			}

			// extract extensions from string
			{
				std::istringstream ss(extensions);
				std::copy(
					util::separated_istream_iterator<std::string>(ss, ','),
					util::separated_istream_iterator<std::string>(),
					std::back_inserter(encoder.extensions));
			}

			// insert encoder into list
			auto iter(
				encoders.insert(
					std::upper_bound(
						encoders.begin(),
						encoders.end(),
						encoder,
						std::greater<Encoder>()),
					encoder));

			// associate encoder with its formats
			for (const auto &format : encoder.formats)
			{
				auto encoders(formatToEncoder[format]);
				encoders.insert(
					std::upper_bound(
						encoders.begin(),
						encoders.end(),
						iter,
						[](const decltype(iter) &a, const decltype(iter) &b)
						{
							return *a > *b;
						}),
					iter);
			}

			// associate encoder with its extensions
			for (const auto &extension : encoder.extensions)
			{
				auto encoders(extensionToEncoder[extension]);
				encoders.insert(
					std::upper_bound(
						encoders.begin(),
						encoders.end(),
						iter,
						[](const decltype(iter) &a, const decltype(iter) &b)
						{
							return *a > *b;
						}),
					iter);
			}
		}

		template <typename T, typename... Args> template <typename U>
			void Factory<T, Args...>::Register(const Key &key, Priority priority)
		{
			Register(
				[](Args &&... args) { return std::unique_ptr<T>(new U(std::forward<Args>(args)...)); },
				key,
				priority);
		}

		/*-----------+
		| production |
		+-----------*/

		template <typename T, typename... Args>
			std::unique_ptr<T> Factory<T, Args...>::Make(Args &&... args) const
		{
			for (const auto &type : types)
			{
				try
				{
					return type.function(std::forward<Args>(args)...);
				}
				catch (...) {}
			}
			THROW((err::Exception<err::UtilModuleTag>("factory failed to make product")))
		}

		template <typename T, typename... Args>
			std::unique_ptr<T> Factory<T, Args...>::Make(const Key &key, Args &&... args) const
		{
			auto iter = keyMap.find(key);
			if (iter == keyMap.end())
				THROW((err::Exception<err::UtilModuleTag, err::FactoryTag, err::KeyNotFound>("product key not registered")));
			return (*iter)(std::forward<Args...>(args...));
		}

		/*-------------+
		| data members |
		+-------------*/

		bool EncoderFactory::operator ==(const Encoder &a, const Encoder &b)
		{
			return a.priority == b.priority;
		}

		bool EncoderFactory::operator <(const Encoder &a, const Encoder &b)
		{
			return a.priority < b.priority;
		}
	}
}
