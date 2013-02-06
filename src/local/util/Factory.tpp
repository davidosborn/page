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
#include <utility> // forward

// local
#include "../err/Exception.hpp"

namespace page
{
	namespace util
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

		template <typename T, typename... Args>
			auto Factory<T, Args...>::GetKeys() const -> std::vector<Key>
		{
			std::vector<Key> keys;
			keys.reserve(types.size());
			std::transform(types.begin(), types.end(), std::back_inserter(keys),
				[](const Type &type) { return type.key; });
			return keys;
		}

		/*-------------+
		| registration |
		+-------------*/

		template <typename T, typename... Args>
			void Factory<T, Args...>::Register(const Function &function, const Key &key, Priority priority)
		{
			keyMap[key] = types.insert(
				std::upper_bound(types.begin(), types.end(), priority,
					[](const Type &type, Priority priority) { return type.priority > priority; }),
				{function, key, priority});
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
	}
}
