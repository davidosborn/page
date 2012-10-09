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

#include "../../res/Index.hpp" // GetIndex, Index::Load
#include "../../res/type/registry.hpp" // GetRegisteredTypeName
#include "../../util/typeinfo.hpp" // GetTypeId

namespace page
{
	namespace cache
	{
		// construct
		template <typename T> Resource<T>::Resource(const std::string &path) :
			path(path) {}

		// clone
		template <typename T> Resource<T> *Resource<T>::Clone() const
		{
			return new Resource<T>(*this);
		}

		// attributes
		template <typename T> std::string Resource<T>::GetType() const
		{
			return res::GetRegisteredTypeName(util::GetTypeId<T>());
		}
		template <typename T> std::string Resource<T>::GetSource() const
		{
			return path;
		}

		// dependency satisfaction
		template <typename T> Resource<T>::operator bool() const
		{
			return !path.empty();
		}

		// instantiation
		template <typename T> typename Resource<T>::Instance Resource<T>::Make() const
		{
			return res::GetIndex().Load<T>(path);
		}
	}
}
