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
			auto MonoFactory<T, Args...>::GetGlobalInstance() -> MonoFactory &
		{
			static MonoFactory instance;
			return instance;
		}
		
		/*-------------+
		| registration |
		+-------------*/

		template <typename T, typename... Args>
			void MonoFactory<T, Args...>::Register(const Function &function)
		{
			if (this->function)
				BOOST_THROW_EXCEPTION(err::Exception("factory function already registered"));
			this->function = function;
		}

		template <typename T, typename... Args> template <typename U>
			void MonoFactory<T, Args...>::Register()
		{
			Register([](Args &&... args) { return std::unique_ptr<T>(new U(std::forward<Args>(args)...)); });
		}

		/*-----------+
		| production |
		+-----------*/

		template <typename T, typename... Args>
			std::unique_ptr<T> MonoFactory<T, Args...>::Make(Args &&... args) const
		{
			if (!function)
				THROW((err::Exception<err::UtilModuleTag, err::FactoryTag, err::NotAvailableTag>("no factory function registered")));
			return function(std::forward<Args>(args)...);
		}
	}
}
