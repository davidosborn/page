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

#include "../../util/class/typeinfo.hpp" // GetIncompleteTypeInfo
#include "../../util/functional/cast.hpp" // reinterpret_cast_function
#include "../../util/memory/VoidDeleterAdaptor.hpp"

namespace page { namespace res
{
////////// PostLoader //////////////////////////////////////////////////////////

	template <typename T>
		PostLoader::PostLoader(const std::function<void (T &)> &f) :
			f(std::bind(f, std::bind(
				util::reinterpret_cast_function<T &, Referenceable &>(),
				std::placeholders::_1))) {}

	template <typename T>
		void PostLoader::operator ()(T &t) const
	{
		(*this)(&t);
	}

////////// TypeRegistry ////////////////////////////////////////////////////////

	template <typename T>
		void TypeRegistry::Register(
			std::string                     const& name,
			std::function<void (T &)>       const& postLoader,
			std::function<void (const T *)> const& deleter)
	{
		Register(
			util::GetIncompleteTypeInfo<T>(),
			Record(name, postLoader, util::VoidDeleterAdaptor<T>(deleter)));
	}

	template <typename T>
		const typename TypeRegistry::Record &TypeRegistry::Query() const
	{
		return Query(util::GetIncompleteTypeInfo<T>());
	}
}}
