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

#include <utility> // forward

#include "../../util/class/typeinfo.hpp" // GetIncompleteTypeInfo
#include "../../util/functional/cast.hpp" // reinterpret_cast_function
#include "../../util/memory/Deleter.hpp" // {,Default}Deleter

namespace page { namespace res { namespace type {

////////// PostLoader //////////////////////////////////////////////////////////

	template <typename T>
		PostLoader::PostLoader(const std::function<void (T &)> &f) :
			f(std::bind(f, std::bind(
				util::reinterpret_cast_function<T &, Referenceable &>(),
				std::placeholders::_1))) {}

	template <typename T>
		void PostLoader::operator ()(T &t) const
	{
		if (f)
			f(reinterpret_cast<Referenceable &>(t));
	}

////////// Record //////////////////////////////////////////////////////////////

	template <typename T>
		Record::Record(
			const std::string &name,
			const std::function<void (T &)> &postLoader) :
				name(name),
				postLoader(postLoader),
				deleter(util::DefaultDeleter<T>()) {}

////////// Registry ////////////////////////////////////////////////////////////

	template <typename T, typename... RecordArgs>
		void Registry::Register(RecordArgs &&... recordArgs)
	{
		Register(
			util::GetIncompleteTypeInfo<T>(),
			Record(std::forward<RecordArgs>(recordArgs)...));
	}

}}}
