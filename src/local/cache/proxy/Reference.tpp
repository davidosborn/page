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

#include "../../util/memory/Deleter.hpp" // Share

namespace page { namespace cache
{
	/*--------------------------+
	| constructors & destructor |
	+--------------------------*/

	template <typename T>
		Reference<T>::Reference(const T &reference) :
			instance(util::Share(reference)) {}

	template <typename T>
		Reference<T>::Reference(const Instance &instance) :
			instance(instance) {}

	/*-------+
	| access |
	+-------*/

	template <typename T>
		typename Reference<T>::Instance Reference<T>::Lock() const
	{
		assert(*this);
		return instance;
	}

	/*----------+
	| observers |
	+----------*/

	template <typename T>
		std::string Reference<T>::GetType() const
	{
		return "reference";
	}

	template <typename T>
		std::string Reference<T>::GetSource() const
	{
		return *this ? "nowhere" : "";
	}

	template <typename T>
		Reference<T>::operator bool() const
	{
		return instance;
	}

	/*----------+
	| modifiers |
	+----------*/

	template <typename T>
		void Reference<T>::Invalidate() const {}

	template <typename T>
		void Reference<T>::Purge() const {}

	/*--------------+
	| instantiation |
	+--------------*/

	template <typename T>
		typename Reference<T>::Instance Reference<T>::Make() const
	{
		assert(!"should never be called");
		return instance;
	}
}}
