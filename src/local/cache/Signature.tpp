/**
 * @copyright
 *
 * Copyright (c) 2006-2014 David Osborn
 *
 * Permission is granted to use and redistribute this software in source and
 * binary form, with or without modification, subject to the following
 * conditions:
 *
 * 1. Redistributions in source form must retain the above copyright notice,
 *    this list of conditions, and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions, and the following disclaimer in the same place
 *    and form as other copyright, license, and disclaimer information.
 *
 * 3. Redistributions in binary form must also include an acknowledgement in the
 *    same place and form as other acknowledgements (such as the credits),
 *    similar in substance to the following:
 *
 *       Portions of this software are based on the work of David Osborn.
 *
 * This software is provided "as is", without any express or implied warranty.
 * In no event will the authors be liable for any damages arising out of the use
 * of this software.
 */

#include <sstream> // ostringstream
#include <utility> // forward

#include "../util/type_traits/range.hpp" // is_range
#include "../util/type_traits/sfinae.hpp" // ENABLE_IF

namespace page { namespace cache
{
	/*-------------+
	| constructors |
	+-------------*/

	namespace detail
	{
		template <typename T>
			void SerializeDependency(std::ostream &os, T x,
				ENABLE_IF(!util::is_range<T>::value))
		{
			os << x;
		}

		template <typename InputRange>
			void SerializeDependency(std::ostream &os, InputRange range,
				ENABLE_IF(util::is_range<InputRange>::value))
		{
			for (const auto &x : range)
				SerializeDependency(os, x);
		}

		template <typename Arg>
			void MakeSource(std::ostream &os, Arg &&arg)
		{
			SerializeDependency(os, std::forward<Arg>(arg));
		}

		template <typename Arg, typename Arg2, typename... Args>
			void MakeSource(std::ostream &os, Arg &&arg, Arg2 &&arg2, Args &&... args)
		{
			SerializeDependency(os, std::forward<Arg>(arg));
			MakeSource(os << ',',
				std::forward<Arg2>(arg2),
				std::forward<Args>(args)...);
		}
	}

	template <typename... Args>
		Signature::Signature(const std::string &type, Args &&... args)
	{
		std::ostringstream ss;
		ss << type << '(';
		detail::MakeSource(ss, std::forward<Args>(args)...);
		ss << ')';
		s = ss.str();
	}
}}
