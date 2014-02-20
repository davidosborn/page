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

#include <utility> // forward

#include "../../util/class/typeinfo.hpp" // GetIncompleteTypeInfo
#include "../../util/functional/cast.hpp" // reinterpret_cast_function

namespace page { namespace res
{
////////// Saver ///////////////////////////////////////////////////////////////

	template <typename T>
		Saver::Saver(const std::function<void (const T &, std::ostream &)> &f) :
			f(std::bind(f, std::bind(
				util::reinterpret_cast_function<const T &, const Referenceable &>(),
				std::placeholders::_1), std::placeholders::_2)) {}

	template <typename T>
		void Saver::operator ()(const T &t, std::ostream &os) const
	{
		(*this)(&t, os);
	}

////////// SaverRegistry ///////////////////////////////////////////////////////

	template <typename T, typename... RecordArgs>
		void SaverRegistry::Register(RecordArgs &&... recordArgs)
	{
		Register(
			util::GetIncompleteTypeInfo<T>(),
			Record(std::forward<RecordArgs>(recordArgs)...));
	}

	template <typename T>
		std::string SaverRegistry::Save(
			T           const& resource,
			std::string        path,
			std::string const& format,
			bool               bestMatch) const
	{
		return Save(
			util::GetIncompleteTypeInfo<T>(),
			&resource, path, format, bestMatch);
	}

	template <typename T>
		const typename SaverRegistry::Record &SaverRegistry::GetSaver(
			std::string const& format,
			std::string const& extension,
			bool               bestMatch) const
	{
		return GetSaver(
			util::GetIncompleteTypeInfo<T>(),
			format, extension, bestMatch);
	}
}}
