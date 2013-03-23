/**
 * @section license
 *
 * Copyright (c) 2006-2013 David Osborn
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

#include "Identifiable.hpp"

namespace page
{
	namespace util
	{
		// construct/copy/destroy/assign
		Identifiable::Identifiable() : id(MakeId())
		{
			GetIds().insert(std::make_pair(id, this));
		}
		Identifiable::Identifiable(const Identifiable &other) : id(MakeId())
		{
			GetIds().insert(std::make_pair(id, this));
		}
		Identifiable::~Identifiable()
		{
			GetIds().erase(id);
		}
		Identifiable &Identifiable::operator =(const Identifiable &) {}

		// identification
		Identifiable::Id Identifiable::GetId() const
		{
			return id;
		}
		Identifiable *Identifiable::FromId(Id id)
		{
			Ids::const_iterator iter(GetIds().find(id));
			return iter != GetIds().end() ? iter->second : 0;
		}

		// identity generation
		Identifiable::Id Identifiable::MakeId()
		{
			static Id id = 1;
			return id++;
		}

		// identity mapping
		Identifiable::Ids &Identifiable::GetIds()
		{
			static Ids ids;
			return ids;
		}
	}
}
