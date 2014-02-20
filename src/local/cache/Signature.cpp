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

#include <functional> // hash
#include <istream>
#include <ostream>

#include "Signature.hpp"

namespace page { namespace cache
{
	/*-------------+
	| constructors |
	+-------------*/

	Signature::Signature(std::nullptr_t) {}

	/*----------+
	| observers |
	+----------*/

	std::string &Signature::str()
	{
		return s;
	}

	const std::string &Signature::str() const
	{
		return s;
	}

	Signature::operator bool() const
	{
		return !s.empty();
	}

	std::string Signature::GetType() const
	{
		return s.substr(0, s.find('(') - 1);
	}

	std::string Signature::GetSource() const
	{
		auto i = s.find('(') + 1;
		return s.substr(i, s.size() - i - 1);
	}

	/*----------------------------+
	| stream insertion/extraction |
	+----------------------------*/

	std::ostream &operator <<(std::ostream &os, const Signature &signature)
	{
		return os << signature.str();
	}

	std::istream &operator >>(std::istream &is, Signature &signature)
	{
		return is >> signature.str();
	}

	/*-----------+
	| comparison |
	+-----------*/

	bool operator ==(const Signature &a, const Signature &b)
	{
		return a.str() == b.str();
	}

	bool operator !=(const Signature &a, const Signature &b)
	{
		return a.str() != b.str();
	}

	bool operator <(const Signature &a, const Signature &b)
	{
		return a.str() < b.str();
	}

	bool operator >(const Signature &a, const Signature &b)
	{
		return a.str() > b.str();
	}

	bool operator <=(const Signature &a, const Signature &b)
	{
		return a.str() <= b.str();
	}

	bool operator >=(const Signature &a, const Signature &b)
	{
		return a.str() >= b.str();
	}
}}

////////// std::hash<Signature> ////////////////////////////////////////////////

namespace std
{
	auto hash<::page::cache::Signature>::operator ()(
		const argument_type &x) const noexcept -> result_type
	{
		return hash<string>()(x.str());
	}
}
